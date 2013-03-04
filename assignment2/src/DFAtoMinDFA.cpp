#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <set>

#include "../include/macros.h"

using namespace std;

void constructDfa ( char DFAinput[1024], vector < vector < map < char, int  > > > *dfa , vector < map < int, string > > *acceptClassType, set < int > *final, set < int > *nonfinal , set < char > *alphabets, vector < set < int > > *tokenStateSets );
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

void arrayprint ( vector < vector < char > > *Distinct, int numStates)
{
	cout<<"\t";
	for ( int i = 0; i < numStates; i++ )
		cout<<i<<"\t";
	cout<<endl;
	cout<<"\t";
	for ( int i = 0; i < 7*numStates; i++ )
		cout<<"_";
	cout<<endl;
	// main
	for ( int i = 0; i < numStates; i++ )
	{
		cout<<i<<".\t|";
		for ( int j = 0; j < numStates; j++ )
			cout<<(*Distinct)[i][j]<<":\t";
		cout<<endl;
	}
	cout<<"\t|";
	for ( int i = 0; i < 7*numStates; i++ )
		cout<<"_";
	cout<<endl;
}

int main()
{
	vector < map < int, string > > validTokenType;
	vector < vector < map < char, int  > > > dfa;
	vector < string > symbolTable;
	vector < set < int > > uniqueStates;
	vector < set < int > > tokenStateSets;
	set < int > final;
	set < int > nonfinal;
	set < char > alphabets;
	char DFAinput[1024];
	strcpy (DFAinput, INPUT );
	strcat ( DFAinput , "simpleDFA" );
	
	// Reconstruct DFA
	// DFA should not modified later
	constructDfa ( DFAinput, &dfa , &validTokenType, &final, &nonfinal , &alphabets, &tokenStateSets );

	/* Now Do DFA Minimization */
	int numStates = dfa[0].size();	//number of states in the dfa
	cout<<numStates<<"\n";
	// Declare 'Distinct' vector
	vector < vector < char > > Distinct;
	Distinct.resize(numStates);
	for ( int i =0; i < numStates; i++ )
		Distinct[i].resize(numStates);
	/*
	for ( int i = 0; i < numStates; i++ )
		for ( int j = 0; j < numStates; j++ )
			Distinct[i][j] = '\0';
	for ( set < int >::iterator itF = final.begin(); itF != final.end(); itF++ )
	{
		for ( set < int >::iterator itNF = nonfinal.begin(); itNF != nonfinal.end(); itNF++ )
		{
			
			Distinct[*itF][*itNF] = '$';
			Distinct[*itF][*itNF] = '$';
		}
	}*/
	
	for ( int i = 0; i < numStates ; i++ )
	{
		for ( int j = 0; j < numStates ; j++ )
			if ( i != j )
				Distinct[i][j] = '$';
			else
				Distinct[i][j] = '\0';
	}
	
	//arrayprint(&Distinct, numStates);
	for ( vector < set < int > >::iterator it = tokenStateSets.begin(); it != tokenStateSets.end(); it++ )
	{
		for ( set < int >::iterator itF = (*it).begin(); itF != (*it).end(); itF++ )
		{
			for ( set < int >::iterator itNF = (*it).begin(); itNF != (*it).end() ; itNF++ )
			{
				Distinct[*itF][*itNF] = '\0';
				Distinct[*itNF][*itF] = '\0';
			}
		}
	}

	arrayprint(&Distinct, numStates);

	// create Distinct table
	bool tableChanged = true;
	while ( tableChanged )
	{
		tableChanged = false;
		for ( int i = 0; i < numStates; i++ )
		{
			for ( int j = 0; j < numStates; j++ )
			{
				for ( set < char >::iterator it = alphabets.begin(); it != alphabets.end(); it++ )
				{
					int p, q;
					p = dfa[0][i][*it];
					q = dfa[0][j][*it];
					if ( Distinct[i][j] == '\0' && Distinct[p][q] != '\0' )
					{
						Distinct[i][j] = *it;
						tableChanged = true;
						//cout<<"I:"<<i<<"\tJ:"<<j<<"\t"<<Distinct[i][j]<<endl;
					}
				}
			}
		}
	}
	arrayprint(&Distinct, numStates);

	// create minimal DFA
	for ( int i = 0; i < numStates; i++ )
	{
		// create set i;
		set < int > similarStates;
		similarStates.insert(i);
		for ( int j = i+1; j < numStates; j++ )
		{
			if ( Distinct[i][j] == '\0' )
			{
				//add to set i
				similarStates.insert(j);
			}
		}
		uniqueStates.push_back(similarStates);
	}
	
	//create minimal DFA using unique sets
	for ( vector < set < int > >::iterator it = uniqueStates.begin(); it != uniqueStates.end(); it++ )
	{
		for ( set < int >::iterator itS = (*it).begin(); itS != (*it).end(); itS++ )
		{
			cout<<*itS<<"\t";
		}
		cout<<endl;
	}






	//constructin input and output filenames
	/*char inputSample[1024];
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
	createSymTabFile( &symbolTable );*/
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
void constructDfa ( char DFAinput[1024], vector < vector < map < char, int  > > > *dfa , vector < map < int, string > > *validTokenType, set < int > *final, set < int > *nonfinal, set < char > *alphabets , vector < set < int > > *tokenStateSets )
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
				set < int > acceptStateSets;
				pch = strtok ( line, "<,>" );
				cout<<"Final states:";
				while ( pch != NULL )
				{
					if ( strcmp (pch, "!") == 0 )
					{
						(*tokenStateSets).push_back ( acceptStateSets );
						acceptStateSets.clear();
						pch = strtok ( NULL, "<,>" );
						if ( pch == NULL )
							break;
						strcpy (tokenType, pch);
						printf ( "\n%s\t", tokenType );	//accepting token type
						pch = strtok ( NULL, "<,>" );
					}
					strcpy (state, pch);
					printf ( "%s\t", state );	//state
					(*final).insert(atoi(state));
					acceptStateSets.insert(atoi(state));
					(*validTokenType)[DFAno][atoi(state)] = tokenType;	//add to acceptClassType
					pch = strtok ( NULL, "<,>" );
					//cout<<"PCH"<<pch[0]<<"\n";
				}
				cout<<"\nTransition functions:\n";
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
					(*alphabets).insert(input[0]);	//add all alphabets
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
		// create non final state set
		for ( int i = 0; i < MaxState; i++ )
		{
			set < int >::iterator it = (*final).find(i);
			if ( it == (*final).end() )
			{
				(*nonfinal).insert(i);
			}
			
		}
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
