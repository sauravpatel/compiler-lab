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

void constructDfa ( char DFAinput[1024], vector < vector < map < char, int  > > > *dfa , vector < map < int, string > > *acceptClassType);
pair < int, int > tokenize ( char *token, vector < vector < map < char,int > > > dfa );
int addToSymTab ( char *token, vector < string > *symbolTable);
void constructLexeme ( char *classType, int index ,  char *lexeme );
void createSymTabFile (  vector < string > *symbolTable );

/* convert integer to string */
/* create full dfa (done)
 * tokenize until accepted (now doing)
 */
string intToStr(int num)
{
	stringstream ss;
	ss<<num;
	return ss.str();
}

int main()
{
	vector < map < int, string > > validTokenType;
	vector < vector < map < char, int  > > > dfa;
	vector < string > symbolTable;
	char DFAinput[1024];
	strcpy (DFAinput, INPUT );
	strcat ( DFAinput , "dfainput" );
	
	// Reconstruct DFA
	constructDfa ( DFAinput, &dfa , &validTokenType);
	/* Now Do Lexical Analysis */
	//constructin input and output filenames
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
				//cout<<pch<<"\n";
				strcpy(token,pch);
				pair < int, int > acceptState = tokenize( token , dfa);	// return type < acceptstate, accepting DFA number >
				//cout<<"TokenId:"<<tokenId<<" ";
				if ( acceptState.first != -1 )
				{
					// add to symbol table
					string tokenType;
					tokenType = validTokenType[acceptState.second][acceptState.first];
					int index = addToSymTab ( token, &symbolTable);
					char lexeme[1024];
					constructLexeme( (char*)tokenType.c_str() , index , lexeme );
					codeOutput<<lexeme;
				}
				else
				{
					cout<<"Invalid token \'"<< pch << "\' found at line "<< linenumber <<"\n";
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
 * returns pair of  < accepting state, accepting DFAno > on success
 * and < -1, numDFA > on invalid token
 */
pair < int, int > tokenize ( char *token , vector < vector< map < char,int > > > dfa)
{
	pair < int, int > acceptState;
	int numDFA = dfa.size();
	int accept = 0;
	int DFAno = 0;
	int nextState;
	while ( !accept && DFAno < numDFA )
	{
		char *current = token;
		char curinput = current[0];
		int curState = 0;
		while( *current++ )
		{
			cout<<curState<<"\t";
			cout<<curinput<<"\t";
			map<char,int>::iterator it = dfa[DFAno].at(curState).find(curinput);
			if ( it == dfa[DFAno][curState].end() )
			{
				cout<<"No transitions available in DFA "<<DFAno<<" Checking Next DFA.\n";
				nextState = -1;
				break;
			}
			nextState = it->second;
			curState = nextState;
			cout << nextState << "\n";
			curinput = current[0];
		}
		if ( nextState != -1 )
		{
			accept = 1;
			break;
		}
		DFAno++;
	}
	acceptState = make_pair ( nextState, DFAno );
	return acceptState;
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

/* Bring DFA to memory from given DFAfile
 * current delimiter for different DFA is : '!'
 */
void constructDfa ( char DFAinput[1024], vector < vector < map < char, int  > > > *dfa , vector < map < int, string > > *validTokenType)
{
	fstream dfainput;
	dfainput.open(DFAinput);
	int DFAno = -1;	

	if ( dfainput.is_open() )
	{
		char curState[1024], nextState[1024];
		char line[1024];
		int MaxState;
		while ( dfainput.getline ( line, 1024 ) )
		{
			//if ( !line )
			//	continue;
			//cout<<"LINE:"<<line<<"\n";
			if ( line[0] == '!' )
			{
				DFAno++;
				(*dfa).resize( DFAno+1 );
				(*validTokenType).resize( DFAno+1 );
				MaxState = 0;
				// mapping between final state and token class
				char state[1024];
				char tokenType[1024];
				char *pch;
				pch = strtok ( line, "<,>" );
				cout<<"Final states:\n";
				while ( pch != NULL )
				{
					if ( strcmp (pch, "!") == 0 )
					{
						pch = strtok ( NULL, "<,>" );
						continue;
					}
					strcpy (state, pch);
					printf ( "%s  ", state );	//state
					pch = strtok ( NULL, "<,>" );
					strcpy (tokenType, pch);
					printf ( "%s\n", tokenType );	//accepting token type
					(*validTokenType)[DFAno][atoi(state)] = tokenType;	//add to acceptClassType
					pch = strtok ( NULL, "<,>" );
					//cout<<"PCH"<<pch[0]<<"\n";
				}
				cout<<"Transition functions:\n";
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
						(*dfa)[DFAno].resize(MaxState);
					}
					cout<<"curstate : "<<curState<<"\n";
				}
				else
				{
					//transition for a state continued
					char input[1024],nextState[1024];
					char *pch = strtok ( line, "," );
					cout<<pch<<"  ";	//current input
					strcpy ( input, pch );
					pch = strtok ( NULL, "");
					strcpy ( nextState, pch );
					cout<<nextState<<"\n";	//next state
					// Resize dfa to maximum possible states
					if ( MaxState < (atoi(nextState) + 1 ) )
					{
						MaxState = atoi(nextState) +1;
						(*dfa)[DFAno].resize(MaxState);
					}
					(*dfa)[DFAno][atoi(curState)][input[0]] = atoi(nextState);
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
