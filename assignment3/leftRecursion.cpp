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
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>

#define LINESIZE 100000

using namespace std;

void printProdRules( map< string, vector< vector< string > > > prodRules )
{
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


map< string, vector< vector< string > > > immediateLeft( string LHS, vector< vector< string > > oneRule )
{
  map< string, vector< vector< string > > > twoRules;
  vector< vector< string > > recursive, nonRecursive, newRule;
  for( vector< vector< string > >::iterator itRule = oneRule.begin(); itRule != oneRule.end(); itRule++ ){
    //cout << " Comparator :" << (*itRule)[0] << endl;
    if( (*itRule)[0] == LHS ){
      recursive.push_back( *itRule );
      //cout << (*itRule)[1] << "EE1    ";
    }
    else{
      nonRecursive.push_back( *itRule );
      //cout << (*itRule)[0] << " :EE2    ";
    }
  }
  if( recursive.size() == 0 ){
    twoRules[ LHS ] = oneRule;
  }
  else{
    string newLHS = LHS + '1';
    for( vector< vector< string > >::iterator itnonRecur = nonRecursive.begin(); itnonRecur != nonRecursive.end(); itnonRecur++ ){
      (*itnonRecur).push_back( newLHS );
      //cout << "non recurrent " << (*itnonRecur)[0] << endl;
      newRule.push_back( *itnonRecur );
    }
    if( newRule.size() != 0 )
      twoRules[ LHS ] = newRule;
    newRule.clear();
    for( vector< vector< string > >::iterator itRecur = recursive.begin(); itRecur != recursive.end(); itRecur++ ){
      (*itRecur).erase( (*itRecur).begin() );
      //cout << "recurrent " << (*itRecur)[0] << endl;
      (*itRecur).push_back( newLHS );
      newRule.push_back( *itRecur );
    }
    vector< string > eps;
    eps.push_back( "epsilon" );
    newRule.push_back( eps );
    twoRules[ newLHS ] = newRule;
  }
  return twoRules;
}

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
    //cout << "Production rules are:" << endl;
    string LHS = "";
    while( getline( readGrammar, line ) ){
      lineNumber++;
      vector< string > oneRule;
      unsigned pos = line.find( "-->" );
      if( pos+1 != 0 ){
        LHS = line.substr( 0,pos );
        line = line.substr( pos+3 );
      }
      else{
        pos = line.find( "|" );
	if( pos == string::npos ){
          cout << "Invalid format for production rule in line number " << lineNumber << endl;
	  exit(0);
	}
        line = line.substr( pos + 1 );
      }
      //cout << "LHS is : " << LHS << "\t";
      //oneRule.push_back( prodRules[ LHS ] );
      istringstream iss( line );
      string RHScomponent="";
      while( iss ){
        iss >> RHScomponent;
        //cout << RHScomponent << "\t";
        oneRule.push_back( RHScomponent );
      }
      //cout << endl;
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
    /*for( map< string, vector< vector< string > > >::iterator itNonTerm = prodRules.begin(); itNonTerm != prodRules.end(); itNonTerm++ ){
      cout << itNonTerm->first << " \t: ";
      for( vector< vector< string > >::iterator itoneRule = itNonTerm->second.begin(); itoneRule != itNonTerm->second.end(); itoneRule++ ){
        for( vector< string >::iterator it=(*itoneRule).begin(); it != (*itoneRule).end(); ++it)
          cout << ' ' << *it;
        cout << " \n\t| ";
      }
      cout << endl;
    }*/
  }
  else{
    cout << "Error in opening file: " << argv[1] << endl;
    exit(0);
  }

  // Now the production rules are in memory

  for( map< string, vector< vector< string > > >::iterator itProdRules = prodRules.begin(); itProdRules != prodRules.end(); itProdRules++ ){
    map< string, vector< vector< string > > > newProdRules;
    newProdRules = immediateLeft( itProdRules->first, itProdRules->second );
    prodRules.erase( itProdRules->first );
    for( map< string, vector< vector< string > > >::iterator itnewProdRules = newProdRules.begin(); itnewProdRules != newProdRules.end(); itnewProdRules++ ){
      prodRules[ itnewProdRules->first ] = itnewProdRules->second;
    }
  }

  // grammar must be without epsilon-production and without cycles
  unsigned numNonTerms = prodRules.size();
  for( map< string, vector< vector< string > > >::iterator iti= prodRules.begin(); iti!= prodRules.end(); iti++ ){
    for( map< string, vector< vector< string > > >::iterator itj = prodRules.begin(); itj != iti; itj++ ){
      
    }
  }

  printProdRules( prodRules );
}
