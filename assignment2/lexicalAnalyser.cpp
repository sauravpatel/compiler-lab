#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>

using namespace std;

void constructDfa ( char filename[1024], vector < map < char, int  > > *dfa , map < int, string > *acceptClassType);
int tokenize ( char *token, vector < map < char,int > > dfa );
int addToSymTab ( char *token, vector < string > *symbolTable);
void constructLexeme ( char *classType, int index ,  char *lexeme );

// convert integer to string
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
	char filename[1024] = "dfainput";
	
	// Reconstruct DFA
	constructDfa ( filename, &dfa , &validTokenType);
		
	// Now Do Lexical Analysis
	fstream codeInput, codeOutput;
	codeInput.open("codeinput");
	codeOutput.open("codeOutput", fstream::out | fstream::trunc );
	char curInput;
	if ( codeInput.is_open () && codeOutput.is_open() )
	{
		char line[1024];
		while ( codeInput.getline(line, 1024) )
		{
			char *pch = strtok( line, " \t\n");
			while ( pch != NULL )
			{
				//cout<<pch<<"\n";
				if ( int tokenId = tokenize( pch , dfa) )
				{
					cout<<"TokenId:"<<tokenId<<" ";
					if ( tokenId != -1 )
					{
						// add to symbol table
						string tokenType;
						tokenType = validTokenType[tokenId];
						int index = addToSymTab ( pch, &symbolTable);
						char lexeme[1024];
						constructLexeme( (char*)tokenType.c_str() , index , lexeme );
						codeOutput<<lexeme;
					}
					else
					{
						cout<<"Invalid token found.\n";
					}
				}
				pch = strtok ( NULL , " \t\n" );
			}
			codeOutput<<endl;
		}
		codeInput.close();
		codeOutput.close();
	}

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
	int curState = 0;
	int nextState;
	while(*current)
	{
		cout<<"CurState: "<<curState<<" ";
		cout<<"CurInput: "<<current[0]<<" ";
		//nextState = getNextState ( curState, current[0] );
		map<char,int>::iterator it = dfa[curState].find(current[0]);
		if ( it == dfa[curState].end() )
		{
			cout<<"Invalid token.\n\n";
			nextState = -1;
			break;
		}
		nextState = it->second;
		curState = nextState;
		cout << "NextState: "<< nextState << "\n";
		current++;
	}
	return nextState;
}

/* Add token to symbol Table
 */
int addToSymTab ( char *token,  vector < string > *symbolTable )
{
	int index = 0;
	int size = (*symbolTable).size();
	cout<<"Size :"<<size;
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
void constructDfa ( char filename[1024], vector < map < char, int  > > *dfa , map < int, string > *validTokenType)
{
	fstream dfainput;
	dfainput.open(filename);
	
	if ( dfainput.is_open() )
	{
		char curstate[1024], nextstate[1024];
		char line[1024];
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
					strcpy( curstate, pch );
					(*dfa).resize(atoi(curstate)+1);
					cout<<"curstate"<<curstate<<"\n";
				}
				else
				{
					//transition for a state continued
					char input[1024],nextstate[1024];
					char *pch = strtok ( line, "," );
					cout<<"input:"<<pch<<"\t";
					strcpy ( input, pch );
					pch = strtok ( NULL, "");
					cout<<"nextstate:"<<pch<<"\n";
					strcpy ( nextstate, pch );
					(*dfa)[atoi(curstate)][input[0]] = atoi(nextstate);
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
