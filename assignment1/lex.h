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
#define IF			12	/* if 							*/
#define THEN		13	/* then							*/
#define DO			14	/* do							*/
#define WHILE		15	/* while						*/
#define BEGIN		16	/* begin						*/
#define END			17	/* end							*/

extern char *yytext;	/* in lex.c						*/
extern int yyleng;
extern yylineno;
