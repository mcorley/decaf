#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Symbols.hh"
#include "AstSymbols.hh"
#include "Ast.hh"
#include "Error.hh"
#include "Headers.hh"

extern FILE *yyin;
extern int yyparse();

extern int yylineno;
extern char* yytext;

int numerrors = 0;
Program *program;
EntityTable *global_symtab;
ClassEntity* objectclass;
Error *error;

char* sourcefile;
ofstream codefile;

void yyerror(const char *errtype)
{
  fprintf(stderr, "%s:%d: %s near token `%s'\n", sourcefile, 
                yylineno, errtype, yytext);
  numerrors++;
}

void initialize() {
  global_symtab = new EntityTable();
  objectclass = new ClassEntity(" Object ", NULL, new list<Entity*>());
  error = new Error(sourcefile);
  initialize_table_data();
}

int main(int argc, char* argv[])
{
  char* destfile;
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(0);
  }

  if ((yyin = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Error in opening file %s\n", argv[1]);
    exit(0);
  } else {
    sourcefile = (char *)malloc(strlen(argv[1])+1);
    strcpy(sourcefile, argv[1]);
  }
  destfile = (char *)malloc(strlen(sourcefile) + 2); // .c
  strcpy(destfile, sourcefile);
  char *dot = strstr(destfile, ".decaf");
  if (dot == NULL)
    strcat(destfile, ".c");
  else
    strcpy(dot, ".c");

  codefile.open(destfile, ios::out);
  if (!codefile) {
    fprintf(stderr, "Error in output file %s\n", destfile);
    exit(0);
  }
    
  initialize();
  if (yyparse() || (numerrors > 0)) {
    fprintf(stderr, "%s contains one or more syntax errors.\n", argv[1]);
  }
  else {
    program->typecheck();
    if (error->hasErrors())
      exit(0);
    program->allocate();
    generate_headers(program);
    generate_main(program);
    program->code();
  }
  codefile.close();
}

