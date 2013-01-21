#define EOI			0	/* End of input					*/
#define SEMI		1	/* ; 							*/
#define PLUS 		2	/* + 							*/
#define TIMES		3	/* * 							*/
#define LP			4	/* (							*/
#define RP			5	/* )							*/
#define NUM_OR_ID	6	/* Decimal Number or Identifier */
/*
 * Added these operations for assignment 1 on 20th January
 */
#define MINUS		7	/* + 							*/
#define DIV 		8	/* + 							*/
#define RELEQUAL	9	/* = (relational check			*/
#define LESS		10	/* <							*/
#define MORE		11	/* >							*/

extern char *yytext;	/* in lex.c						*/
extern int yyleng;
extern yylineno;
