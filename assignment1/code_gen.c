#include <stdio.h>
#include "lex.h"
#include <stdlib.h>

char    *factor     ( void );
char    *term       ( void );
char    *expression ( void );

extern char *newname( void       );
extern void freename( char *name );
/*
 * Added Boolean expressions for relational checking in if-else
 * statements
 */
char 	*relExp()
{
	/*
	 * relExp -> expression RELEQUAL relExp
	 * 			| expression LESS relExp
	 * 			| expression MORE relExp
	 * 			| expression
	 */
	 char *tempvar, *tempvar2, *tempvar3;
	 tempvar = expression();
	 int count = 1;
	 while( match( RELEQUAL ) || match( LESS ) || match( MORE ) )
	 {
		count++;
		int flagEQUAL = match ( RELEQUAL );
		int flagLESS = match ( LESS );
		int flagMORE = match ( MORE );
		
		advance();
        tempvar2 = expression();
		tempvar3 = newname();
        if (flagEQUAL)
			printf("    %s = %s == %s\n", tempvar3, tempvar, tempvar2 );
        if ( flagLESS )
			printf("    %s = %s < %s\n", tempvar3, tempvar, tempvar2 );
		if ( flagMORE )
			printf("    %s = %s > %s\n", tempvar3, tempvar, tempvar2 );
		freename( tempvar );
		freename( tempvar2 );
		tempvar = tempvar3;
    }
    return tempvar;
}
constructs()
{
	/* constructs 	-> IF relExp THEN oneStmt
     * 				| WHILE relExp DO oneStmt
     * 				| BEGIN optStatements 
     * BEGIN and END are used as { and } for multiline statements in
     * THEN or DO
     */
    char *tempvar;
	if ( match ( IF ) ){
		advance();
		tempvar = relExp();
		if ( match ( THEN ) ){
			advance();
			printf("    if %s then\n", tempvar );
			oneStmt();
		}
		else
			printf ( "Missing 'then' for 'if' construct\n");
		freename (tempvar);
	}
	if ( match ( WHILE ) ){
		advance();
		tempvar = relExp();
		if ( match ( DO ) ){
			advance();
			printf("    while %s do\n", tempvar );
			oneStmt();
			freename ( tempvar );
		}
		else {
			printf ( "Missing 'do' for 'while' construct\n");
			freename ( tempvar );
		}
	}
	if ( match ( BEGIN ) ){
		advance();
		printf("    begin:\n" );
		optStatements();
	}
}

optStatements()
{	/* 
	 * optStatements -> statements END
	 */
	
	while ( !match ( END ) )
		oneStmt();
	printf("    end\n" );
	advance();
}

oneStmt(){
	/*	oneStmt	->constructs
	 * 			| expression SEMI
	 * 			| ID_ASSIGN expression SEMI
	 */
	if ( match (IF) || match ( WHILE ) || match ( BEGIN ) )
		constructs();
	else{
		if ( match ( ID_ASSIGN ) ){
			char *tempvar, *tempvar2;
			advance();
			tempvar = expression();
			printf("    %s := %s\n", tempvar2 = newname(), tempvar  );
			freename(tempvar);
			if ( match ( SEMI ) )
				advance();
			else
				fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno );
		}
		else {
			char *tempvar;
			tempvar = expression();
			freename (tempvar);
			if( match( SEMI ) )
				advance();
			else
				fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno );
			}
	}
}

statements()
{
    /*  
     * statements	->	oneStmt statements
     * 					| epsilon
     * statements represents stmt-list described in assignment1
     */
    while( !match(EOI) )
    	oneStmt();
}

char    *expression()
{
    /* expression -> term expression'
     * expression' -> PLUS term expression' |  epsilon
     * 				| MINUS term expression'
     */

    char  *tempvar, *tempvar2;

    tempvar = term();
    
    while( match( PLUS ) || match( MINUS ) )
    {
		int flagPLUS = match ( PLUS );
		advance();
        tempvar2 = term();
        if (flagPLUS)
			printf("    %s += %s\n", tempvar, tempvar2 );
        else
			printf("    %s -= %s\n", tempvar, tempvar2 );
		
        freename( tempvar2 );
    }

    return tempvar;
}

char    *term()
{
	/*
	 * term -> factor factor'
	 * factor' -> TIMES factor'
	 * 			| DIV factor'
	 */	
    char  *tempvar, *tempvar2 ;

    tempvar = factor();
    while( match( TIMES ) || match ( DIV ) )
    {
		int flagTIMES = match ( TIMES );
        advance();
        tempvar2 = factor();
        if ( flagTIMES )
			printf("    %s *= %s\n", tempvar, tempvar2 );
        else
        	printf("    %s /= %s\n", tempvar, tempvar2 );
        
        freename( tempvar2 );
    }

    return tempvar;
}

char    *factor()
{
    char *tempvar;

    if( match(NUM_OR_VAR) )
    {
	/* Print the assignment instruction. The %0.*s conversion is a form of
	 * %X.Ys, where X is the field width and Y is the maximum number of
	 * characters that will be printed (even if the string is longer). I'm
	 * using the %0.*s to print the string because it's not \0 terminated.
	 * The field has a default width of 0, but it will grow the size needed
	 * to print the string. The ".*" tells printf() to take the maximum-
	 * number-of-characters count from the next argument (yyleng).
	 */
		printf("    %s = %0.*s\n", tempvar = newname(), yyleng, yytext );
        advance();
    }
    else if( match(LP) )
    {
        advance();
        tempvar = expression();
        if( match(RP) )
            advance();
        else
            fprintf(stderr, "%d: Mismatched parenthesis\n", yylineno );
    }
    else
	fprintf( stderr, "%d: Number or identifier expected\n", yylineno );

    return tempvar;
}
