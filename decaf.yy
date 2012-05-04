%{
#include <cstdio>
#include <iostream>
#include "ast.h"
using namespace std;

// Prototypes to lexer functions
extern int yylex(); 
extern void yyerror(const char *s);

// Interface to the outside world
extern list<Entity *> *toplevel;   // list of top-level classes
extern EntityTable *global_symtab; // global symbol table
extern ClassEntity* objectclass;   // top-level "Object" class

// Global vars to simulate certain inherited attributes
Type *current_type;

%}
/* Declarations */

/* Reserved words */
%token TOK_BOOLEAN
%token TOK_BREAK 
%token TOK_CONTINUE 
%token TOK_CLASS 
%token TOK_DO 
%token TOK_ELSE
%token TOK_EXTENDS 
%token TOK_FALSE 
%token TOK_FLOAT 
%token TOK_FOR 
%token TOK_IF 
%token TOK_INT 
%token TOK_NEW 
%token TOK_NULL 
%token TOK_PRIVATE 
%token TOK_PUBLIC 
%token TOK_RETURN 
%token TOK_STATIC
%token TOK_SUPER 
%token TOK_THIS 
%token TOK_TRUE 
%token TOK_VOID 
%token TOK_WHILE 

/* Operators */
%token TOK_COMMA
%token TOK_SEMICOLON
%token TOK_LPAREN
%token TOK_RPAREN
%token TOK_LBRACE
%token TOK_RBRACE
%token TOK_LBRACK
%token TOK_RBRACK
%token TOK_DOT
%token TOK_INCR 
%token TOK_DECR

/* Precedence and Associativity */
%right    TOK_ASSIGN
%left     TOK_OR
%left     TOK_AND
%left     TOK_EQ TOK_NE 
%nonassoc TOK_GT TOK_LT TOK_GE TOK_LE
%left     TOK_PLUS TOK_MINUS
%left     TOK_MULT TOK_DIV
%left     TOK_NOT

/* Errors */
%token TOK_SYNTAX_ERROR TOK_RUNAWAY_STRING

/* These have a value */
%token TOK_IDENT
%token TOK_INTNUM
%token TOK_FLOATNUM
%token TOK_STRCONST

/* Attribute types */
%union {
  bool                bVal;
  char                *cVal;
  float               dVal;
  int                 uVal;
  ClassEntity         *classEntity;
  Entity              *entity;
  Expression          *expression;
  MethodEntity        *methodEntity;  
  list<Entity *>      *entityList;
  list<Expression *>  *exprList;
  list<Statement *>   *stmtList;
  pair<bool, bool>    *modifierVal;
  Statement           *statement;
  Type                *typeVal;
  VariableEntity      *varEntity;  
};

/* Non-terminal type info */
%type <cVal> TOK_IDENT TOK_STRCONST
%type <uVal> TOK_INTNUM dim_star
%type <dVal> TOK_FLOATNUM
%type <bVal> visibility_opt static_opt
%type <modifierVal> modifier
%type <entity> class_decl class_body_decl constructor_decl field_decl
%type <entityList> class_decls class_body_decls 
%type <entityList> formal_param_list formals_opt
%type <entityList> var_decl var_list variables
%type <classEntity> extends_opt class_head
%type <methodEntity> method_decl method_head
%type <varEntity> variable formal_param
%type <exprList> dim_expr_plus arguments_opt expr_list_star
%type <stmtList> stmt_star
%type <expression> expr expr_opt assign method_invocation literal lhs
%type <expression> primary field_access array_access dim_expr
%type <statement> block stmt else_opt stmt_expr_opt stmt_expr
%type <typeVal> type

/* Start-symbol */
%start program

%expect 1 /* The famous "dangling `else'" ambiguity 
             results in one shift/reduce conflict   
             that we don't want to be reported  */

/* Grammer Rules */
%%

/********************************/
/*         Decaf Program        */
/********************************/

program
  : class_decls
    {
      // In the action for this production, set 
	    // the global variable "toplevel" to the list of entities representing 
	    // the classes (i.e. the attribute of class_declarations).
      toplevel = $1;
    }
  ;


/********************************/
/*         Declarations         */
/********************************/

class_decls
  : /* empty */ { 
      $$ = new list<Entity *>(); 
    }

  | class_decls class_decl {
      $$ = $1;
      $$->push_back($2);
    }
  ;

class_decl
  : class_head TOK_LBRACE class_body_decls TOK_RBRACE 
    {
      if ($1)
        $1->set_class_members($3);
      $$ = $1;
      global_symtab->leave_block();
    }
  ;

class_head
  : TOK_CLASS TOK_IDENT extends_opt
    {
      bool current;
      Entity *entity = global_symtab->find_entity($2, CLASS_ENTITY, &current);
      ClassEntity *classEntity = dynamic_cast<ClassEntity *>(entity);
      if (classEntity) {
        yyerror("Redefinition of class");
      } else {
        classEntity = new ClassEntity($2, $3, new list<Entity *>());
      }
      $$ = classEntity;
      global_symtab->enter_block();
    }
  ;

extends_opt
  : /* We're a base class */ { 
      $$ = objectclass; 
    }

  | TOK_EXTENDS TOK_IDENT
    {
      bool current;
      Entity *entity = global_symtab->find_entity($2, CLASS_ENTITY, &current);
      ClassEntity *superclass = dynamic_cast<ClassEntity *>(entity);
      if (!superclass) {
        yyerror("Undeclared superclass");
        $$ = objectclass;
      } else { 
        $$ = superclass;
      }
    }
  ;

class_body_decls
  : class_body_decls class_body_decl {
      $$ = $1; 
      $$->push_back($2);
    }

	| class_body_decl {
      $$ = new list<Entity *>(); 
      $$->push_back($1);
    }
  ;

class_body_decl
  : field_decl { $$ = $1; }
  | method_decl { $$ = $1; }
  | constructor_decl { $$ = $1; }
  ;

field_decl
  : modifier type TOK_IDENT dim_star TOK_SEMICOLON
    {
      bool current;
      Entity *entity = global_symtab->find_entity($3, FIELD_ENTITY, &current);
      FieldEntity *fieldEntity = dynamic_cast<FieldEntity *>(entity);
      if (fieldEntity && current) {
        yyerror("Redefinition of field name");
      } else {
        fieldEntity = new FieldEntity($3, $1->first, $1->second, $2, $4);
      }
      $$ = fieldEntity;
    }
  ;

modifier
	: visibility_opt static_opt {
      $$ = new pair<bool, bool>($1, $2);   
    }
  ;

visibility_opt
  : /* default */ { $$ = true; }
  | TOK_PUBLIC { $$ = true; }
  | TOK_PRIVATE { $$ = false; }
  ;

static_opt
  : /* default */ { $$ = false; }
  | TOK_STATIC { $$ = true; }
  ;

var_decl 
  : type variables TOK_SEMICOLON
    {
      list<Entity *>::const_iterator it = $2->begin();
      for (; it != $2->end(); ++it) {
        VariableEntity *var = dynamic_cast<VariableEntity *>(*it);
        var->set_type($1);
      }
      $$ = $2;
    }
  ;

type
  : TOK_INT { $$ = new IntType(); }
  | TOK_FLOAT { $$ = new FloatType(); }
  | TOK_BOOLEAN { $$ = new BooleanType(); }
  | TOK_IDENT   
    {
      bool current;
      Entity *entity = global_symtab->find_entity($1, CLASS_ENTITY, &current);
      ClassEntity *classEntity = dynamic_cast<ClassEntity *>(entity);
      if (classEntity) {
        $$ = new InstanceType(classEntity);
      } else {
        yyerror("Undeclared class type");
        $$ = new ErrorType();
      }
    }
  ;

variables
  : variable var_list {
      $$ = $2;
      $$->push_front($1);      
    }
  ;

variable
  : TOK_IDENT dim_star
    {
      bool current;
      Entity *entity = global_symtab->find_entity($1, VARIABLE_ENTITY, &current);
      VariableEntity *variable = dynamic_cast<VariableEntity *>(entity);
      if (variable && current) {
        yyerror("Redefinition of variable name");       
      } else {
        // Postpone filling in type information
        variable = new VariableEntity($1, NULL, $2);        
      }
      $$ = variable;
    }
  ;

var_list
  : /* empty */ {
      $$ = new list<Entity *>();
    }

  | TOK_COMMA variable var_list {
      $$ = $3;
      $$->push_front($2);
    }
  ;

method_decl
  : method_head TOK_LPAREN formals_opt TOK_RPAREN block
    {
      $$ = $1;
      $$->set_formal_params($3);
      $$->set_method_body($5);
      global_symtab->leave_block();
    }
  ;

method_head
  : modifier type TOK_IDENT {
      $$ = new MethodEntity($3, $1->first, $1->second, $2, NULL, NULL);
      global_symtab->enter_block();
    }

  | modifier TOK_VOID TOK_IDENT {
      $$ = new MethodEntity($3, $1->first, $1->second, new VoidType(), NULL, NULL);
      global_symtab->enter_block();
    }
  ;

formals_opt
  : /* empty */ {
      $$ = new list<Entity *>();
    }

  | formal_param formal_param_list {
      $$ = $2;
      $$->push_front($1);
    }
  ;

formal_param
  : type variable {
      $$ = $2;
      $$->set_type($1);
    }
  ;

formal_param_list
  : /* empty */ {
      $$ = new list<Entity *>();   
    }

  | TOK_COMMA formal_param formal_param_list {
      $$ = $3;
      $$->push_front($2);
    }
  ;

constructor_decl
  : modifier TOK_IDENT TOK_LPAREN { global_symtab->enter_block(); } 
        formal_param_list TOK_RPAREN block {
      $$ = new ConstructorEntity($2, $1->first, $5, $7);
      global_symtab->leave_block();
    }
  ;

/********************************/
/*         Statements           */
/********************************/

block
  : TOK_LBRACE { global_symtab->enter_block(); } stmt_star TOK_RBRACE {
      $$ = new BlockStatement($3);
      global_symtab->leave_block();
    }
  ;

stmt_star
  : /* empty */ {
      $$ = new list<Statement *>();
    }

  | stmt stmt_star {
      $$ = $2;
      $$->push_front($1);
    }
  ;

stmt
  : TOK_IF TOK_LPAREN expr TOK_RPAREN stmt else_opt {
      $$ = new IfStatement($3, $5, $6);
    }

  | TOK_WHILE TOK_LPAREN expr TOK_RPAREN stmt {
      $$ = new WhileStatement($3, $5);
    }

  | TOK_FOR TOK_LPAREN stmt_expr_opt TOK_SEMICOLON expr_opt 
        TOK_SEMICOLON stmt_expr_opt TOK_RPAREN stmt {
      $$ = new ForStatement($3, $5, $7, $9);
    }

  | TOK_RETURN expr TOK_SEMICOLON { $$ = new ReturnStatement($2); }
  | stmt_expr TOK_SEMICOLON { $$ = $1; }
  | block { $$ = $1;}
  | var_decl { $$ = new DeclStatement($1); }
  | TOK_BREAK TOK_SEMICOLON { $$ = new BreakStatement(); }
  | TOK_CONTINUE TOK_SEMICOLON { $$ = new ContinueStatement(); }
  | TOK_SEMICOLON { $$ = new SkipStatement(); }
  | error TOK_SEMICOLON { $$ = new SkipStatement(); }
  ;

else_opt
  : /* empty */ { $$ = new SkipStatement(); }
  | TOK_ELSE stmt { $$ = $2; }
  ;

stmt_expr_opt
  : /* empty */ { $$ = new SkipStatement(); }
  | stmt_expr { $$ = $1; }
  ;

expr_opt
  : /* empty */ { $$ = new BooleanConstant(true); }
  | expr { $$ = $1; }
  ;

stmt_expr
  : assign { $$ = new ExprStatement($1); }
  | method_invocation { $$ = new ExprStatement($1); }
  ;


/********************************/
/*       Expressions            */
/********************************/

primary 
  : literal { $$ = $1; }
  | TOK_THIS { $$ = new ThisExpression(); }
  | TOK_SUPER { $$ = new SuperExpression(); }
  | TOK_LPAREN expr TOK_RPAREN { $$ = $2; }
  | lhs { $$ = $1; }
  | method_invocation { $$ = $1; }
  | TOK_NEW TOK_IDENT TOK_LPAREN arguments_opt TOK_RPAREN 
    {
      bool current;
      Entity *entity = global_symtab->find_entity($2, CLASS_ENTITY, &current);
      ClassEntity *classEntity = dynamic_cast<ClassEntity *>(entity);
      if (!classEntity)
        yyerror("Undeclared class");
      $$ = new NewInstance(classEntity, $4);
    }
  ;

arguments_opt
  : /* empty */ {
      $$ = new list<Expression *>();
    }

  | expr expr_list_star 
    {
      $$ = $2;
      $$->push_front($1);
    }
  ;

expr_list_star
  : /* empty */ { 
      $$ = new list<Expression *>(); 
    }

  | TOK_COMMA expr expr_list_star 
    {
      $$ = $3;
      $$->push_front($2);
    }
  ;

field_access
  : primary TOK_DOT TOK_IDENT {
      $$ = new FieldAccess($1, $3);
    }
    
  | TOK_IDENT 
    {
      bool current;
      Entity *entity = global_symtab->find_entity($1, VARIABLE_ENTITY, &current);
      if (entity && current) { 
        // Local variable
        VariableEntity *local = dynamic_cast<VariableEntity *>(entity);
        $$ = new IdExpression(local);
      } else if (entity = global_symtab->find_entity($1, FIELD_ENTITY, &current)) {
        // Field variable
        FieldEntity *field = dynamic_cast<FieldEntity *>(entity);
        $$ = new IdExpression(field);
      } else if (entity = global_symtab->find_entity($1, CLASS_ENTITY, &current)) {
        // Class variable
        ClassEntity *classEntity = dynamic_cast<ClassEntity *>(entity);
        $$ = new IdExpression(classEntity);
      } else {
        // Unresolved
        $$ = new FieldAccess(new ThisExpression(), $1);
      }
    }
  ;

array_access
  : primary TOK_LBRACK expr TOK_RBRACK {
      $$ = new ArrayAccess($1, $3);
    }
  ;

method_invocation
  : primary TOK_DOT TOK_IDENT TOK_LPAREN arguments_opt TOK_RPAREN {
      $$ = new MethodInvocation($1, $3, $5);
    }

  | TOK_IDENT TOK_LPAREN arguments_opt TOK_RPAREN {
      $$ = new MethodInvocation(new ThisExpression(), $1, $3);
    }
  ;

expr
  : expr TOK_PLUS expr { $$ = new BinaryExpression(ADD, $1, $3); }
  | expr TOK_MINUS expr { $$ = new BinaryExpression(SUB, $1, $3); }
  | expr TOK_MULT expr { $$ = new BinaryExpression(MUL, $1, $3); }
  | expr TOK_DIV expr { $$ = new BinaryExpression(DIV, $1, $3); }
  | expr TOK_AND expr { $$ = new BinaryExpression(AND, $1, $3); }
  | expr TOK_OR expr { $$ = new BinaryExpression(OR, $1, $3); }
  | expr TOK_EQ expr { $$ = new BinaryExpression(EQ, $1, $3); }
  | expr TOK_NE expr { $$ = new BinaryExpression(NEQ, $1, $3); }
  | expr TOK_GT expr { $$ = new BinaryExpression(GT, $1, $3); }
  | expr TOK_LT expr { $$ = new BinaryExpression(LT, $1, $3); }
  | expr TOK_GE expr { $$ = new BinaryExpression(GEQ, $1, $3); }
  | expr TOK_LE expr { $$ = new BinaryExpression(LEQ, $1, $3); }
  | TOK_NOT expr { $$ = new UnaryExpression(NEG, $2); }
  | TOK_MINUS expr %prec TOK_NOT { $$ = new UnaryExpression(UMINUS, $2); }
  | TOK_PLUS expr  %prec TOK_NOT { $$ = $2; } 
  | primary { $$ = $1; }
  | assign { $$ = $1; }
  | TOK_NEW type dim_expr_plus dim_star {
      const int dimension = $3->size() + $4;
      $$ = new NewArrayInstance($2, dimension, $3); 
    }
  ;

dim_expr_plus
  : dim_expr {
      $$ = new list<Expression *>();
      $$->push_back($1);
    }

  | dim_expr_plus dim_expr {
      $$ = $1;
      $$->push_back($2);
    }
  ;

dim_expr
  : TOK_LBRACK expr TOK_RBRACK { $$ = $2; }
  ;

dim_star
  : /* scalar */ { $$ = 0; }
  | dim dim_star { $$ = $2 + 1; }
  ;

dim
  : TOK_LBRACK TOK_RBRACK {}
  ;

assign
  : lhs TOK_ASSIGN expr { $$ = new AssignExpression($1, $3); }
  | lhs TOK_INCR { $$ = new AutoExpression(POST_INCR, $1); }
  | lhs TOK_DECR { $$ = new AutoExpression(POST_DECR, $1); }
  | TOK_INCR lhs { $$ = new AutoExpression(PRE_DECR, $2); }
  | TOK_DECR lhs { $$ = new AutoExpression(PRE_INCR, $2); }
  ;

lhs
  : field_access { $$ = $1; }
  | array_access { $$= $1; }
  ;

literal
  : TOK_INTNUM { $$ = new IntegerConstant($1); }
  | TOK_FLOATNUM { $$ = new FloatConstant($1); }
  | TOK_STRCONST { $$ = new StringConstant($1); }
  | TOK_NULL { $$ = new NullExpression(); }
  | TOK_TRUE { $$ = new BooleanConstant(true); }
  | TOK_FALSE { $$ = new BooleanConstant(false); }
  ;

%%

