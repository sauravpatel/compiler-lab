/****************************************
* Name: first follow set
* Description: Create first and follow set for a given grammar
* By: Saurav Kumar Patel
****************************************/

/**************************************
* Input file format:
* first line contains list of terminals
* second line contains list of all non-Terminals
* Rest file contains production rules
* Each line emust contaion only one production rule
* For example if you have rule like : (A-->bB | d), the it should be written as
* (A-->bB)
* (A-->d), in two seperate lines
**************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <set>

#define LINESIZE 10000

using namespace std;

// print set
void printSet( set< string > myset ){
  for( set< string >::iterator it = myset.begin(); it != myset.end(); it++ ){
    cout << *it << ", ";
  }
  cout <<endl;
}


int main( int argc, char** argv )
{
  if( argc < 3 ){
    cout << "Usage: ./program_name inputGrammar_File Output_file\n";
    exit(0);
  }

  set <string> terms, nonTerms;
  int lineNumber=0;  // For error reporting during reading input grammar file
  fstream readGrammar, outputfile;
  string line="";
  vector< list< string > > prodRules;  // First entry is LHS and rest is part of RHS in a list

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
      istringstream iss( line );
      cout << "List of all Non-terminals are: ";
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
    while( getline( readGrammar, line ) ){
      lineNumber++;
      list< string > oneRule;
      unsigned pos = line.find( "-->" );
      if( pos != string::npos ){
        string LHS = line.substr( 0,pos );
	//cout << LHS << "\t";
	oneRule.push_back( LHS );
	line = line.substr( pos+3 );
	istringstream iss( line );
	string RHScomponent="";
	while( iss ){
	  iss >> RHScomponent;
          //cout << RHScomponent << "\t";
	  oneRule.push_back( RHScomponent );
	}
	oneRule.pop_back();
        // Print rules as in vector prodRules
        for (list< string >::iterator it=oneRule.begin(); it != oneRule.end(); ++it)
          cout << ' ' << *it;
	cout << endl;
	if( oneRule.size() != 1 ){
	  prodRules.push_back( oneRule );
	}
	else{
          cout << "RHS missing for production rule in line number " << lineNumber;
	  exit(0);
	}
      }
      else{
        cout << "Invalid format for production rule in line number " << lineNumber;
	exit( 0 );
      }
    }
  }
  else{
    cout << "Error in opening file: " << argv[1] << endl;
    exit(0);
  }

  /* Now production rules are in memory.
   * The following codes will generate first and follow sets.
   */

   map< string, set< string > > firstSet, followSet;

   // Generate first set of all terminals
   for( set< string >::iterator it = terms.begin(); it != terms.end(); it++ ){
     firstSet[ *it ].insert( *it );
   }
   firstSet[ "epsilon" ].insert( "epsilon" );  // for epsilon
   //printSet( terms );

   // Generate first set of all non terminals
   //int loopNo = 1;
   bool change = true;
   while( change ){
     //cout << "Loop Number : " << loopNo++ << endl;
     change = false;
     int lineNumber = 2;
     for( vector< list< string > >::iterator itOneRule = prodRules.begin(); itOneRule != prodRules.end(); itOneRule++ ){
       lineNumber++;
       list< string > oneRule = *itOneRule;
       string LHS = oneRule.front();
       oneRule.pop_front();
       unsigned prevSize = firstSet[ LHS ].size();
       for( list< string >::iterator itRHS = oneRule.begin(); itRHS != oneRule.end(); itRHS++ ){
         if( terms.find( *itRHS ) != terms.end() ){
           firstSet[ LHS ].insert( *itRHS );
  	   //cout <<LHS << " : ";
	   //printSet( firstSet[ LHS ] );
	   if ( prevSize != firstSet[ LHS ].size() ) 
	     change = true;
	   break;
         }
	 else if( nonTerms.find( *itRHS ) != nonTerms.end() ){
	   for( set< string >::iterator itfirstSet = firstSet[ *itRHS ].begin(); itfirstSet != firstSet[ *itRHS ].end(); itfirstSet++ ){
	     firstSet[ LHS ].insert( *itfirstSet );
	     if ( prevSize != firstSet[ LHS ].size() )
  	       change = true;
	   }
	   //cout<<LHS<<;
	   //printSet( firstSet[ LHS ] );
	   if( firstSet[ *itRHS ].find( "epsilon" ) != firstSet[ *itRHS ].end() ){
	     //cout<<"found epsilon for "<< LHS<<" because of "<<*itRHS<<"\n";
	   }
	   else
             break;
	 }
	 else{
	   cout << "Invalid symbol '" << *itRHS << "' found in line number " << lineNumber << endl;
	   exit( 0 );
	 }
       }
     }
   }
   //printing first set
   cout << "\nAll first sets are:\n";
   for ( map< string, set< string > >::iterator it = firstSet.begin(); it != firstSet.end(); it++ ){
     cout << it->first <<" : ";
     printSet( it->second );
   }

   // Now generate follow sets
}
