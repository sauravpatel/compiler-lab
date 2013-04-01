/**************************************************
 * Name: Eliminating Left Recursion
 * By: Saurav Kumar Patel
 *************************************************/
/**************************************
* Input file format:
* first line contains list of terminals
* second line contains list of all non-Terminals ( first symbol is START symbol )
* Rest file contains production rules
* For example (A-->bB | d), we write
* A-->bB
*     |d
**************************************/


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <map>
#include <list>
#include <set>

#define LINESIZE 100000

using namespace std;

int main( int argc, char **argv )
{
  if( argc != 2 ){
    cout << "Invalid Usage:\n";
    cout << "./program inputGrammar\n";
    exit(0);
  }
  set <string> terms, nonTerms;
  int lineNumber=0;  // For error reporting during reading input grammar file
  fstream readGrammar, outputfile;
  string line="";
  string start="";  // For start symbol
  map< string, vector< vector< string > > > prodRules;  // First entry is LHS and rest is part of multiple RHS in a vector of list

  readGrammar.open( argv[1] );
  if( readGrammar.is_open() ){
    /* Get Terminals and non-Terminals list from first two lines */
    if( getline( readGrammar, line ) ){
      lineNumber++;
      string terminal;
      istringstream iss( line );
      cout << "List of all Terminals are: ";
      while( iss ){
	iss >> terminal;
	cout << terminal << ", ";
	terms.insert( terminal );
      }
      cout<<endl;
      terms.insert( "epsilon" );  // epsilon is a terminal
    }
    else{
      cout << "Missing list of terminals from file" << endl;
      exit(0);
    }
    if( getline( readGrammar, line ) ){
      lineNumber++;
      string nonTerminal;
      cout << "List of all Non-terminals are: ";
      istringstream iss( line );
      iss >> start;  // first non terminal should be start symbol
      nonTerms.insert( start );
      cout << start << ", ";
      while( iss ){
	iss >> nonTerminal;
	cout << nonTerminal << ", ";
	nonTerms.insert( nonTerminal );
      }
      cout<<endl;
    }
    else{
      cout << "Missing list of non terminals from file. Please give input file in corect format." << endl;
      exit(0);
    }

    /* Now comes the time for reading production rules */
    cout << "Production rules are:" << endl;
    string LHS = "";
    while( getline( readGrammar, line ) ){
      lineNumber++;
      vector< string > oneRule;
      unsigned pos = line.find( "-->" );
      if( pos != string::npos )
        LHS = line.substr( 0,pos );
      else{
        pos = line.find( "|" );
	if( pos == string::npos ){
          cout << "Invalid format for production rule in line number " << lineNumber << endl;
	  exit(0);
	}
      }
      //cout << LHS << "\t";
      //oneRule.push_back( prodRules[ LHS ] );
      line = line.substr( pos+3 );
      istringstream iss( line );
      string RHScomponent="";
      while( iss ){
        iss >> RHScomponent;
        //cout << RHScomponent << "\t";
        oneRule.push_back( RHScomponent );
      }
      oneRule.pop_back();
      if( oneRule.size() > 0 ){
        prodRules[ LHS ].push_back( oneRule );
      }
      else{
        cout << "RHS missing for production rule in line number " << lineNumber << endl;
        exit(0);
      }
    }
    // Print rules as in vector prodRules
    for( map< string, vector< vector< string > > >::iterator itNonTerm = prodRules.begin(); itNonTerm != prodRules.end(); itNonTerm++ ){
      cout << itNonTerm->first << " \t: ";
      for( vector< vector< string > >::iterator itoneRule = itNonTerm->second.begin(); itoneRule != itNonTerm->second.end(); itoneRule++ ){
        for( vector< string >::iterator it=(*itoneRule).begin(); it != (*itoneRule).end(); ++it)
          cout << ' ' << *it;
        cout << " \n\t| ";
      }
      cout << endl;
    }
  }
  else{
    cout << "Error in opening file: " << argv[1] << endl;
    exit(0);
  }
}
