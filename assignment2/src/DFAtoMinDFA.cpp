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

string intTOStr( int num )
{
	stringstream ss;
	ss << num;
	return ss.str();
}

int main()
{
	string inputDFA = INPUT;inputDFA += "simpleDFA";
	string minDFA = INPUT;minDFA += "minDFA";
	fstream simpleDFA;
	simpleDFA.open(inputDFA.c_str());
	
	if ( simpleDFA.is_open() )
	{
		int curstate, nextstate;
		string line;
		while(true)
		{
			getline ( simpleDFA, line );
			istringstream lexemeStream (line);
			while (lexemeStream >> lexeme)
			{
				cout << lexeme << ":\n";
			}
			if ( inputFile.eof())
				break;	
			}
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
		simpleDFA.close();
	}
}
