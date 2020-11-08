#include <iostream>
#include <string>
#include <map>

#include "lex.h"

using namespace std;
using std::map;

string stringtoupper(string &s)
{
   for(int i = 0; i < s.length(); i++)
  {
    s[i] = toupper(s[i]);
  }
   return s;
}

map < Token,string > tokenPrint = {

        {  PRINT, "PRINT"},
        {  IF, "IF"},
        {  BEGIN, "BEGIN"},
        {  END, "END"},
        {  IDENT, "IDENT"},
        {  ICONST,"ICONST"},
        {  SCONST,"SCONST"},
        {  RCONST,"RCONST"},
        {  PLUS,"PLUS"},
        {  MINUS,"MINUS"},
        {  MULT,"MULT"},
        {  DIV,"DIV"},
        {  EQ,"EQ"},
        {  LPAREN,"LPAREN"},
        {  RPAREN,"RPAREN"},
        {  SCOMA,"SCOMA"},
        {  COMA,"COMA"},
        {  ERR, "ERR"},
        {  DONE,"DONE"}

    };

map < string, Token> tokenPrint2 = {

        {  "PRINT", PRINT},
        {  "IF", IF},
        {  "BEGIN", BEGIN},
        {  "END", END},
        {  "IDENT", IDENT},
        {  "ICONST",ICONST},
        {  "SCONST",SCONST},
        {  "RCONST",RCONST},
        {  "PLUS",PLUS},
        {  "MINUS",MINUS},
        {  "MULT",MULT},
        {  "DIV",DIV},
        {  "EQ",EQ},
        {  "LPAREN",LPAREN},
        {  "RPAREN",RPAREN},
        {  "SCOMA",SCOMA},
        {  "COMA",COMA},
        {  "ERR", ERR},
        {  "DONE",DONE}

};

ostream& operator<<(ostream& out, const LexItem& tok) {

    Token token = tok.GetToken();
    int linenum = tok.GetLinenum();
    out<<tokenPrint[token];


    if(token == IDENT || token == ICONST || token == SCONST || token == ERR || token == RCONST) {
        out<<" ("<<tok.GetLexeme()<<")";
    }
    out<<" Line # : "<<linenum+1;

    return out;

}

LexItem getNextToken(istream& in, int& linenumber) {

    char ch;
    string lexeme;
    enum TokState { BEGIN, INID, INSTRING, ININT, INREAL, INCOMMENT, SIGN } lexstate = BEGIN;

    while( in.get(ch) ) {

        switch( lexstate ) {

            case BEGIN:
                if( ch == '\n' ) {
                    lexeme += ch;
                    linenumber++;
                    lexeme.clear();
                    }

                if( isspace(ch) ) {
                    continue;
                }

                if( isalpha(ch) ) {
                    lexeme += ch;
                    lexstate = INID;
                }

                if( isdigit(ch) ) {
                    lexeme += ch;
                    if( in.peek() == '.' ) {
                        in.get(ch);
                        lexeme += ch;
                        lexstate = INREAL;
                    }
                    else {

                    lexstate = ININT;
                    }
                }

                if( ch == '"') {
                    lexstate = INSTRING;
                }

                if( ch == '-' ) {
                    lexeme += ch;
                    return LexItem(MINUS, lexeme, linenumber);
                }

                if( ch == '+' ) {
                    lexeme += ch;
                    return LexItem(PLUS, lexeme, linenumber);
                }

                if( ch == '*' ) {
                    lexeme += ch;
                    return LexItem(MULT, lexeme, linenumber);
                    lexeme.clear();
                }

                if( ch == '/' && in.peek() == '/') {
                        lexeme += ch;
                        lexstate = INCOMMENT;
                        in.putback(ch);
                        lexeme.clear();
                }

                 if ( ch == '/' && in.peek() != '/' ) {
                    lexeme += ch;
                    return LexItem(DIV, lexeme, linenumber);
                    lexeme.clear();
                    }




                if( ch == '(' ) {
                    lexeme += ch;
                    return LexItem(LPAREN, lexeme, linenumber);
                    lexeme.clear();
                }

                if( ch == ')' ) {
                    lexeme += ch;
                    return LexItem(RPAREN, lexeme, linenumber);
                    lexeme.clear();
                }

                if( ch == ';' ) {
                    lexeme += ";";
                    return LexItem(SCOMA, lexeme, linenumber);
                    lexeme.clear();
                }

                if( ch == ',' ) {
                    lexeme += ch;
                    return LexItem(COMA, lexeme, linenumber);
                    lexeme.clear();
                }

                break;

            case INID:
                if( isalpha(ch) || isdigit(ch) ) {

                    lexeme += ch;

                }

                else {


                    if( ch == '.' || ch == '!' ) {

                        lexeme.clear();
                        lexeme += ch;

                        return LexItem(Token::ERR, lexeme, linenumber);
                    }

                    string lex = lexeme;

                    if ( tokenPrint2.count(stringtoupper(lex)) > 0 ) {

                        return LexItem( tokenPrint2[lex], lexeme, linenumber);

                    }

                    else {

                        return LexItem( Token::IDENT, lexeme, linenumber);
                    }

                }
                break;

                case ININT:
                    if(isdigit(ch)) {

                        lexeme += ch;

                    }

                    else if(isalpha(ch)) {

                        lexeme += ch;
                        return LexItem(Token::ERR, lexeme, linenumber);

                    }

                    else {

                        in.putback(ch);
                        return LexItem(Token::ICONST, lexeme, linenumber);

                    }

                    break;


                case INSTRING:
                    if( ch == '"' ) {

                        return LexItem(Token::SCONST, lexeme, linenumber);

                    }

                    else if( ch == '\\' ) {

                        if( isalpha(in.peek()) || isspace(in.peek()) || ispunct(in.peek()) ) {

                            if(in.peek() == 'n') {
                                in.get(ch);
                                lexeme += '\n';
                            }

                            else {
                                in.get(ch);
                                lexeme += ch;
                            }
                        }

                        else {
                            lexeme += '\\';
                        }
                    }

                    else if( ch == '\n' ) {
                        lexeme += ch;
                        lexeme.insert(0, 1, '"');
                        return LexItem(Token::ERR, lexeme, linenumber);
                    }

                    else {
                        lexeme += ch;
                    }

                        break;

                    case INCOMMENT:
                        if( ch == '\n' ) {
                            linenumber++;
                            lexstate = BEGIN;
                        }

                        break;

                    case INREAL:
                        if ( isdigit(ch) ) {
                            lexeme += ch;
                        }
                        else {
                            return LexItem(Token::RCONST, lexeme, linenumber);
                            lexeme.clear();
                        }

                        break;

                    // case SIGN:
                    // //add
                    //     break;


                    case DONE:
                        break;

                    case ERR:
                        break;
            }
        }



    if( in.eof() ) {
        return LexItem(Token::DONE, lexeme, linenumber);
    }

    return LexItem();
}