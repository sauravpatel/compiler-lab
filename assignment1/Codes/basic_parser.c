/* Basic parser, shows the structure but there's no code generation */

#include <stdio.h>
#include "lex.h"

int statements(void)
{
    /*  statements -> expression SEMI
     *             |  expression SEMI statements
     */

    expression();

    if( match( SEMI ) )
	advance();
    else
        fprintf( stderr, "%d: Inserting missing semicolon\n", yylineno );

    if( !match(EOI) )
    {
        statements();			/* Do another statement. */
    }
}

/*  new grammar
* expression ~ expr
* expr -> term exp1
* exp1 -> MINUS term exp1
* 		| MINUS term exp2
*       | exp2
* exp2 -> PLUS term exp1
* 		| epsilon

*/


int expression()
{
    /* expr -> term expr1 */

    term();
    exp1();
}

int exp1()
{
	/* 
	 * exp1 -> MINUS term exp1
	 * 		 | MINUS term exp2
	 *       | exp2
	 */

	if(match(MINUS))
	{
		advance();
		term();
		if(match(MINUS))
		{
			advance();
			term();
			exp1();
			printf("-\n");
		}
		expr_prime();
		printf("-\n");
	}
	else
	{
		expr_prime();
	}
}

int expr_prime()
{  
     /* exp2 -> PLUS term exp1
     * 				| epsilon
     */

    if( match( PLUS ) )
    {
		advance();
        term();
        printf("+\n");
        exp1();
    }
}

/* new grammar
 * term -> factor term1
 * term1 -> TIMES factor term1
 * 		  | TIMES factor term2
 * 		  | term2
 * 
 * term2 -> DIV factor term1
 * 		  |epsilon
 */ 


int term()
{
    /* term -> factor term1 */

    factor();
    term1();
}

int term1()
{
	/* newly added
	 * term1 -> TIMES factor term1
	 * 		  | TIMES factor term2
	 * 		  | term2
	 */
	 
	if(match(TIMES))
	{
		advance();
		factor();
		if(match(TIMES))
		{
			advance();
			factor();
			term1();
			printf("*\n");
		}
		term_prime();
		printf("*\n");
	}
	else
	{
		term_prime();
	}
}


int term_prime()
{
    /* term2 -> TIMES factor term1
     * 		  | epsilon
     */

    if( match( DIV ) )
    {
	    advance();
        factor();
        printf("\\\n");
        term1();
    }
}

int factor()
{
    /* factor   ->    NUM_OR_ID
     *          |     LP expression RP
     */

    if( match(NUM_OR_ID) )
        advance();

    else if( match(LP) )
    {
        advance();
        expression();
        if( match(RP) )
            advance();
        else
            fprintf( stderr, "%d: Mismatched parenthesis\n", yylineno);
    }
    else
	fprintf( stderr, "%d Number or identifier expected\n", yylineno );
}
