%option noyywrap
%option never-interactive
%option yylineno
%{
#include "ast.h"
#include "decaf.tab.h"
using namespace std;
int yylinenum = 1;
const char* yyfilename = "";
#define yylex_retval(x,y,z) {return x;}
#define yylex_ret(x) yylex_retval(x, yytext, yyleng)
%}
  /* Definitions */
WS      [\t\r\n ]
ESCAPE  [\\]

ONELINE_COM_BEG     "//"
MULTI_LINE_COM_BEG  "/*"
MULTI_LINE_COM_END  "*/"

ESCAPESEQ	  {ESCAPE}(.|"\n")
DQBODY      [^"\\]|{ESCAPESEQ}
DQSTR	      ["]({DQBODY}*)["]

IDENT_FIRST  [_A-Za-z]
IDENT_REST   [_A-Za-z0-9]
IDENT        ({IDENT_FIRST})(({IDENT_REST})*)

DECIMAL_DIGIT   [0-9]
MANTISSA_EINT   {DECIMAL_DIGIT}*[.]{DECIMAL_DIGIT}+
MANTISSA_EFRAC  {DECIMAL_DIGIT}+(([.]{DECIMAL_DIGIT}*)?)
FLOAT_MANTISSA  ({MANTISSA_EINT}|{MANTISSA_EFRAC})
FLOAT_EXP       [Ee]([+-]?){DECIMAL_DIGIT}+
FLOATNUM        {FLOAT_MANTISSA}({FLOAT_EXP}?)
INTNUM          {DECIMAL_DIGIT}+

%x COMMENT TO_EOL_COMMENT
%%
  /* Reserved words */
"boolean"   { return TOK_BOOLEAN; }
"break"     { return TOK_BREAK; }
"continue"  { return TOK_CONTINUE; }
"class"     { return TOK_CLASS; }
"do"        { return TOK_DO; }
"else"      { return TOK_ELSE; }
"extends"   { return TOK_EXTENDS; }
"false"     { return TOK_FALSE; }
"float"     { return TOK_FLOAT; }
"for"       { return TOK_FOR; }
"if"        { return TOK_IF; }
"int"       { return TOK_INT; }
"new"       { return TOK_NEW; }
"null"      { return TOK_NULL; }
"private"   { return TOK_PRIVATE; }
"public"    { return TOK_PUBLIC; }
"return"    { return TOK_RETURN; }
"static"    { return TOK_STATIC; }
"super"     { return TOK_SUPER; }
"this"      { return TOK_THIS; }
"true"      { return TOK_TRUE; }
"void"      { return TOK_VOID; }
"while"     { return TOK_WHILE; }

  /* Operators */
"++"    { return TOK_INCR; }
"--"    { return TOK_DECR; }
"!"	    { return TOK_NOT; }
"-"	    { return TOK_MINUS; }
"+"	    { return TOK_PLUS; }
"*"	    { return TOK_MULT; }
"/"	    { return TOK_DIV; }
"="	    { return TOK_ASSIGN; }
"&&"    { return TOK_AND; }
"||"    { return TOK_OR; }
">"	    { return TOK_GT; }
"<"	    { return TOK_LT; }
"=="    { return TOK_EQ; }
"!="    { return TOK_NE; }
">="    { return TOK_GE; }
"<="    { return TOK_LE; }
"."	    { return TOK_DOT; }
","	    { return TOK_COMMA; }
";"	    { return TOK_SEMICOLON; }
"["	    { return TOK_LBRACK; }
"]"	    { return TOK_RBRACK; }
"("	    { return TOK_LPAREN; }
")"	    { return TOK_RPAREN; }
"{"	    { return TOK_LBRACE; }
"}"	    { return TOK_RBRACE; }

<<EOF>> {
	BEGIN 0;
	yyterminate();
}

{ONELINE_COM_BEG} {
  BEGIN TO_EOL_COMMENT;
}

<TO_EOL_COMMENT>"\n" {
   BEGIN 0;
}

<TO_EOL_COMMENT>. {
}

{MULTI_LINE_COM_BEG} {
	BEGIN COMMENT;
}

<COMMENT>{MULTI_LINE_COM_END} {
  BEGIN 0;
}

<COMMENT>. {
}

{INTNUM} {
	yylval.uVal = atoi(yytext);
	return TOK_INTNUM;
}

{FLOATNUM} {
	yylval.dVal = atof(yytext);
	return TOK_FLOATNUM;
}

{IDENT} {
	yylval.cVal = strdup(yytext);
	return TOK_IDENT;
}

{DQSTR} {
  // We need to strip off quotes
  yylval.cVal = (char *) malloc(yyleng + 1);
  strncpy(yylval.cVal, yytext + 1, yyleng - 2);
  yylval.cVal[yyleng - 2] = '\0';
 	return TOK_STRCONST;
}

{WS}+ {
}

. { 
	return TOK_SYNTAX_ERROR;
}

%%

