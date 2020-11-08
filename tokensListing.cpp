#include <iostream>
#include "lex.h"
#include "io.cpp"
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <string>

/*
 * CS280 - Fall 2020
 */

#include <cctype>
#include <map>

using namespace std;

int main(int argc, char *argv[]) {

	int linenumber = 0;
	bool v = false;
	bool iconsts = false;
	bool rconsts = false;
	bool sconsts = false;
	bool ids = false;

	istream *input = &cin;

	ifstream inFile;

	for ( int i = 1; i<argc; i++) {

		string vari(argv[i]);

		if(vari == "-v") {
			v = !v;
		}

		else if(vari == "-iconsts") {
			iconsts = !iconsts;
		}

		else if(vari == "-rconsts") {
			rconsts = !rconsts;
		}

		else if(vari == "-sconsts") {
			sconsts = !sconsts;
		}

		else if(vari == "-ids") {
			ids = !ids;
		}

		else if(vari[0] == '-') {
			cerr<<"UNRECOGNIZED FLAG "<<vari<<endl;
		return -1;
		}

		else if( i == argc-1 ) {
			inFile.open(vari);

			if (inFile.is_open() == false) {
				cerr<<"UNABLE TO OPEN FILE "<<vari<<endl;
			return -1;
			}

			input = &inFile;

		}



		else {
			cerr<<"ONLY ONE FILE NAME ALLOWED"<<endl;
			return -1;
		}

	}

	LexItem tok;
	int tokencounter=0;

	list<string> idents;
	list<string> sconst;
	list<string> iconst;
	list<string> rconst;

	while((tok = getNextToken(*input, linenumber)) != Token:: ERR && tok != Token::DONE) {
		//cout<<"FLAG ";
		tokencounter++;

		if (v) {
			//cout<<"FLAG ";
			cout<<tok<<endl;
		}

		if (tok.GetToken() == IDENT) {
			idents.push_back(tok.GetLexeme());
		}

		if (tok.GetToken() == SCONST) {
			sconst.push_back(tok.GetLexeme());
		}

		if (tok.GetToken() == ICONST) {
			iconst.push_back(tok.GetLexeme());
		}

		if (tok.GetToken() == RCONST) {
			rconst.push_back(tok.GetLexeme());
		}
	}

	if(tok.GetToken() == Token:: ERR) {
		cout<< "Error on line "<<linenumber + 1<<" ("<<tok.GetLexeme()<<")"<<endl;
		return -1;
	}

	if(ids) {
		if(idents.size()>0) {
			idents.unique();
			idents.sort();

		cout<<"IDENTIFIERS: ";
			for(string n: idents) {

				cout<<n;

				if(n.compare(idents.back())) {
					cout<<", ";
				}

			}
		cout<<endl;
		}
	}

	if(sconsts) {
		if(sconst.size()>0) {
			sconst.unique();
			sconst.sort();

		cout<<"STRINGS: ";
			for(string n: sconst) {

				cout<<n<<endl;

			}


		}
	}

	if(iconsts) {
		if(iconst.size()>0) {
			iconst.unique();
			iconst.sort();

		cout<<"INTEGERS: ";
			for(string n: iconst) {

				cout<<n<<endl;

				if(!n.compare(iconst.front())) {

				}

			}
		}


	}

	if(rconsts) {
		if(rconst.size()>0) {
			rconst.unique();
			rconst.sort();

			cout<<"REALS: ";
				for(string n: rconst) {

					cout<<n<<endl;

					if(!n.compare(iconst.front())) {
						//cout<<endl;
					}
				}

		}
	}

	cout<<"Total Lines: "<<linenumber-1<<endl;

	if(tokencounter >= 1) {
		cout<<"Total Tokens: "<<tokencounter<<endl;
	}

    return 0;
}
