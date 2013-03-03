#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include "../include/macros.h"

using namespace std;

void constructDfa ( char DFAinput[1024], vector < map < char, int  > > *dfa , map < int, string > *acceptClassType);
int tokenize ( char *token, vector < map < char,int > > dfa );
int addToSymTab ( char *token, vector < string > *symbolTable);
void constructLexeme ( char *classType, int index ,  char *lexeme );
void createSymTabFile (  vector < string > *symbolTable );

/* convert integer to string */
string intToStr(int num)
{
	stringstream ss;
	ss<<num;
	return ss.str();
}

int main()
{
	map < int, string > validTokenType;
	vector < map < char, int  > > dfa;
	vector < string > symbolTable;
	char DFAinput[1024];
	strcpy (DFAinput, INPUT );
	strcat ( DFAinput , "dfainput" );
	
	// Reconstruct DFA
	constructDfa ( DFAinput, &dfa , &validTokenType);
		
	// Now Do Lexical Analysis
	char inputSample[1024];
	char outputSample[1024];
	strcpy ( inputSample, INPUT );
	strcat ( inputSample, "codeinput" );
	strcpy ( outputSample, OUTPUT );
	strcat (outputSample, "codeinput" );
	fstream codeInput, codeOutput;
	codeInput.open(inputSample);
	codeOutput.open(outputSample, fstream::out | fstream::trunc );
	char curInput;
	if ( codeInput.is_open () && codeOutput.is_open() )
	{
		char line[1024];
		int linenumber = 0;
		while ( codeInput.getline(line, 1024) )
		{
			linenumber++;
			char token[1024];
			char *pch = strtok( line, " \t\n");
			while ( pch != NULL )
			{
				strcpy(token,pch);
				//cout<<pch<<"\n";
				if ( int tokenId = tokenize( token , dfa) )
				{
					//cout<<"TokenId:"<<tokenId<<" ";
					if ( tokenId != -1 )
					{
						// add to symbol table
						string tokenType;
						tokenType = validTokenType[tokenId];
						int index = addToSymTab ( token, &symbolTable);
						char lexeme[1024];
						constructLexeme( (char*)tokenType.c_str() , index , lexeme );
						codeOutput<<lexeme;
					}
					else
					{
						cout<<"Invalid token \'"<< pch << "\' found at line "<< linenumber <<"\n";
					}
				}
				pch = strtok ( NULL , " \t\n" );
			}
			codeOutput<<endl;
		}
		codeInput.close();
		codeOutput.close();
	}
	// store symbol table in a file
	createSymTabFile( &symbolTable );
	return 0;
}

/*------------End of Main----------------------*/

/* Finds matching token ID ( i.e. accepting state of token ) corresponding to token
 * returns accepting state on success
 * and -1 on invalid token
 */
int tokenize ( char *token , vector< map < char,int > > dfa)
{
	char *current = token;
	char curinput = current[0];
	int curState = 0;
	int nextState;
	while( *current++ )
	{
		cout<<curState<<"\t";
		cout<<curinput<<"\t";
		map<char,int>::iterator it = dfa.at(curState).find(curinput);
		if ( it == dfa[curState].end() )
		{
			cout<<"No transitions available.\n";
			nextState = -1;
			break;
		}
		nextState = it->second;
		curState = nextState;
		cout << nextState << "\n";
		curinput = current[0];
	}
	return nextState;
}

/* Add token to symbol Table */
int addToSymTab ( char *token,  vector < string > *symbolTable )
{
	int index = 0;
	int size = (*symbolTable).size();
	//cout<<"Size :"<<size;
	for (int i = 0; i < size; i++ )
	{
		if ( (*symbolTable)[i] == string( token ) ) 
		{
			index = i;
			break;
		}
	}
	if ( index == 0 )
	{
		index = size;
		(*symbolTable).push_back( token );
	}
	return index+1;
}

/* Bring DFA to memory from given DFAfile */
void constructDfa ( char DFAinput[1024], vector < map < char, int  > > *dfa , map < int, string > *validTokenType)
{
	fstream dfainput;
	dfainput.open(DFAinput);
	
	if ( dfainput.is_open() )
	{
		char curState[1024], nextState[1024];
		char line[1024];
		int MaxState = 0;
		while ( dfainput.getline ( line, 1024 ) )
		{
			//if ( !line )
			//	continue;
			//cout<<"LINE:"<<line<<"\n";
			if ( line[0] == '<' )
			{
				// mapping between final state and token class
				char state[1024];
				char tokenType[1024];
				char *pch;
				pch = strtok ( line, "<,>" );
				while ( pch != NULL )
				{
					strcpy (state, pch);
					printf ( "state:%s:", state );
					pch = strtok ( NULL, "<,>" );
					strcpy (tokenType, pch);
					printf ( "class:%s\n", tokenType );
					(*validTokenType)[atoi(state)] = tokenType;	//add to acceptClassType
					pch = strtok ( NULL, "<,>" );
				}
			}
			else
			{
				// transition function
				if ( line[0] == '#' )
				{
					//state changed
					char *pch = strtok ( line, "#,:" );
					strcpy( curState, pch );
					if ( MaxState < atoi(curState) + 1 )
					{
						MaxState = atoi(curState) +1;
						(*dfa).resize(MaxState);
					}
					cout<<"curstate"<<curState<<"\n";
				}
				else
				{
					//transition for a state continued
					char input[1024],nextState[1024];
					char *pch = strtok ( line, "," );
					cout<<"input:"<<pch<<"\t";
					strcpy ( input, pch );
					pch = strtok ( NULL, "");
					cout<<"nextstate:"<<pch<<"\n";
					strcpy ( nextState, pch );
					
					// Resize dfa to maximum possible states
					if ( MaxState < atoi(nextState) + 1 )
					{
						MaxState = atoi(nextState) +1;
						(*dfa).resize(MaxState);
					}
					(*dfa)[atoi(curState)][input[0]] = atoi(nextState);
				}
			}
			
		}
		dfainput.close();
	}
}

/* Determines Format of the lexemes to be added in the output file of source code
 * FORMAT : '< TOKEN_TYPE, INDEX >'
 */
void constructLexeme ( char *tokenType, int index , char *lexeme )
{
	strcpy ( lexeme, "< " );
	strcat (lexeme, tokenType );
	strcat ( lexeme, ", " );
	strcat ( lexeme, (char*)intToStr(index).c_str() );
	strcat ( lexeme, " > " );
	cout<<"LEXEME:"<<lexeme<<"\n";
}


void createSymTabFile (  vector < string > *symbolTable )
{
	char symTabFile[1024];
	strcpy ( symTabFile, SYMTAB );
	strcat (symTabFile, "codeinput" );
	fstream symTabPtr ;
	symTabPtr.open(symTabFile , fstream::out | fstream::trunc );
	if ( symTabPtr.is_open () )
	{
		for ( int i = 0; i < (*symbolTable).size(); i++ )
		{
			string entry = intToStr ( i ) + ", " + (*symbolTable)[i] + "\n";
			symTabPtr << entry;
		}
		symTabPtr.close();
	}
}
