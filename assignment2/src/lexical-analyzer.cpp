#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <map>

#include "../include/macros.h"

using namespace std;

char* addToSymbolTable(char *inputfile , char *token, map<string,string> keywords, char*lexemes);
void addToOutputFile(char *lexemes);

int main()
{
	/* Initialise DFA.
	 * Later it will be read from file.
	 */
	vector <map<int,int> > dfa;
		
	// Keywords
	map <string,string> keywords;
	keywords[INT] = "INT";
	keywords[FLOAT] = "INT";
	keywords[LONG] = "INT";
	keywords[DOUBLE] = "INT";
	keywords[IF] = "INT";
	keywords[ELSE] = "INT";
	keywords[FOR] = "INT";
	keywords[WHILE] = "INT";
	keywords[DO] = "INT";
	
	dfa.resize(5);
	// state 0,1 alphabets
	for(int j=0;j<2;j++)
	for(char i = 'a'; i <= 'z'; i++ ){
		dfa[j][i] = 1;
	}
	// state 0 digits
	for(char i='1';i<='9';i++){
		dfa[0][i] = 2;
	}
	// all white spaces
	for (int j=0;j<3;j++){
		dfa[j][' '] = 3;
		dfa[j]['\n']= 3;
		dfa[j]['\t']= 3;
	}
		
	// state 1 digits
	for(char i='0';i<='9';i++)
		dfa[1][i] = 1;
	
	// state 2 alphabets
	for(char i='a'; i<='z'; i++){
		dfa[2][i] = 4;
	}

	//state 2 digits
	for(char i='0';i<='9';i++)
		dfa[2][i] = 2;
	
	//state 4 alphabets
	for(char i='a'; i<='z'; i++){
		dfa[4][i] = 4;
	}
	
	//state 4 digits
	for(char i='0';i<='9';i++)
		dfa[4][i] = 2;
	
	//state 4 whitespace
	dfa[4][' '] = 0;
	dfa[4]['\n'] = 0;
	dfa[4]['\t'] = 0;

	char filename[100];
	char inputfile[100]="test";
	strcpy ( filename, INPUT);
	strcat ( filename, inputfile );
	cout << filename <<"\n";
	
	char input[1020];
	strcpy ( input, "a 2 3aa ag a4 34 ahg ar3t " );
	int curstate = 0;
	char curinput;
	char token[1024];
	char character[10];
	sprintf(character, "%c", ' ');
	strcpy(token," ");
	for ( int i = 0; input[i] != '\0'; i++ ){
		curinput = input[i];
		//cout<<"    current input:"<<curinput;
		switch(curstate){
			case 0:
			{
				curstate = dfa[curstate][curinput];
				sprintf(character, "%c", curinput);
				strcpy(token,character);
				//cout<<"token:"<<token<<":state:"<<curstate;
				break;
			}
			case 1:
			{
				curstate = dfa[curstate][curinput];
				sprintf(character, "%c", curinput);
				strcat(token,character);
				//cout<<"token:"<<token<<":state:"<<curstate;
				break;
			}
			case 2:
			{
				curstate = dfa[curstate][curinput];
				sprintf(character, "%c", curinput);
				strcat(token,character);
				//cout<<"token:"<<token<<":state:"<<curstate;
				break;
			}
			case 3:
			{
				// add token to symbol table
				printf("complete Token :%s:\n",token);
				cout<<strlen(token)<<"length";
				char finalToken[1024];
				strncpy(finalToken,token,22);
				cout<<finalToken<<"?Final";
				char lexemes[1024];
				strcpy(lexemes, addToSymbolTable(inputfile, finalToken ,keywords,lexemes));
				addToOutputFile(lexemes);
				// general operation as in all cases
				curstate = 0;
				curstate = dfa[curstate][curinput];
				sprintf(character, "%c", curinput);
				strcpy(token,character);
				//cout<<"token:"<<token<<":state:"<<curstate;
				break;
			}
			case 4:
			{
				cout<<"Invalid identifier at position "<<i<<"\n";
				while(input[i]!=' '){
					i=i+1;
					cout<<input[i];
				}
				curinput = input[i];
				//cout<<"Currinput:"<<curinput;
				curstate = dfa[curstate][curinput];
				sprintf(character, "%c", curinput);
				strcpy(token,character);
				//cout<<"token:"<<token<<":state:"<<curstate;
				break;
			}
		}
	}
		
		//add last token to symbol table
		if(curstate == 3){
			printf("complete Token :%s\n",token);
				
		}	
	cout<<"\nWorking\n";
	return 0;
}
/***********END OF MAIN***********************/

void addToOutputFile(char *lexemes)
{
	char outfilename[100];
	strcpy ( outfilename, OUTPUT);
	strcat ( outfilename, "/input.cpp" );
	//cout << outfilename <<"\n";
}

char* addToSymbolTable(char *inputfile, char *token, map<string,string> keywords, char *lexemes)
{
	cout<<token<<":token";
	strcpy(lexemes, "<");
	strcat(lexemes,token);

	if(keywords.find(token) != keywords.end()){
			strcat(lexemes,">");
	}
	else
	{
		strcat(lexemes,",");
		char symtabfile[100];
		strcpy ( symtabfile, OUTPUT);
		strcat ( symtabfile, inputfile);
		strcat ( symtabfile, ".symtab");
		cout << symtabfile <<"\n";
		fstream fptr;
		fptr.open(symtabfile, fstream::in | fstream::out);
		char line[1024];
		if(fptr.is_open()){cout<<"File Opened\n";
			if(!fptr.eof()){
				strcat(lexemes,"1>");
				fptr>>lexemes;
				cout<<lexemes;
			}
			while ( !fptr.eof() ){
				fptr.getline(line,1024);
					fptr>>lexemes;
					cout<<line;
			}
			fptr.close();
		}
		else
			cout<<"ERROR Openeing file\n";

	}
		
	return lexemes;
}
