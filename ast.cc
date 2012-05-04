// Ast.cc

#include <iostream>
#include "ast.h"

extern int yylineno;

int Statement::lineno() {
  return lineno_;
}

int Expression::lineno() {
  return lineno_;
}

////////////////////////////////
// 
// IfStatement class
//
////////////////////////////////

IfStatement::IfStatement(Expression* expr, Statement* thenpart, Statement* elsepart)
  : expr_(expr), 
    thenpart_(thenpart), 
    elsepart_(elsepart) 
{
    lineno_ = yylineno;
}

IfStatement::~IfStatement() {}

Expression* IfStatement::expr() {
   return expr_;
}

Statement* IfStatement::thenpart() {
   return thenpart_;
}

Statement* IfStatement::elsepart() {
   return elsepart_;
}


////////////////////////////////
// 
// WhileStatement class
//
////////////////////////////////

WhileStatement::WhileStatement(Expression* expr, Statement* body)
  : expr_(expr),
    body_(body) 
{
    lineno_ = yylineno;
}

WhileStatement::~WhileStatement() {}

Expression* WhileStatement::expr() 
{
   return expr_;
}

Statement* WhileStatement::body() 
{
   return body_;
}


////////////////////////////////
// 
// ForStatement class
//
////////////////////////////////

ForStatement::ForStatement(Statement* init, Expression* guard, 
    Statement* update, Statement* body)
  : init_(init),
    guard_(guard),
    update_(update),
    body_(body) 
{
    lineno_ = yylineno;
}

ForStatement::~ForStatement() {}

Statement* ForStatement::init() 
{
   return init_;
}

Expression* ForStatement::guard() 
{
   return guard_;
}

Statement* ForStatement::update() 
{
   return update_;
}

Statement* ForStatement::body() 
{
   return body_;
}


////////////////////////////////
// 
// ReturnStatement class
//
////////////////////////////////

ReturnStatement::ReturnStatement(Expression* expr)
  : expr_(expr) 
{
    lineno_ = yylineno;
}

ReturnStatement::~ReturnStatement() {}

Expression* ReturnStatement::expr() 
{
   return expr_;
}


////////////////////////////////
// 
// BlockStatement class
//
////////////////////////////////

BlockStatement::BlockStatement(list<Statement*>* stmt_list)
  : stmt_list_(stmt_list) 
{
    lineno_ = yylineno;
}

BlockStatement::~BlockStatement() {}

list<Statement*>* BlockStatement::stmt_list() 
{
   return stmt_list_;
}


////////////////////////////////
// 
// DeclStatement class
//
////////////////////////////////

DeclStatement::DeclStatement(list<Entity*>* var_list)
  : var_list_(var_list) 
{
    lineno_ = yylineno;
}

DeclStatement::~DeclStatement() {}

list<Entity*>* DeclStatement::var_list() 
{
   return var_list_;
}


////////////////////////////////
// 
// ExprStatement class
//
////////////////////////////////

ExprStatement::ExprStatement(Expression* expr)
  : expr_(expr) 
{
    lineno_ = yylineno;
}

ExprStatement::~ExprStatement() {}

Expression* ExprStatement::expr() 
{
   return expr_;
}


////////////////////////////////
// 
// BreakStatement class
//
////////////////////////////////

BreakStatement::BreakStatement() {
    lineno_ = yylineno;
}

BreakStatement::~BreakStatement() {}


////////////////////////////////
// 
// ContinueStatement class
//
////////////////////////////////

ContinueStatement::ContinueStatement() {
    lineno_ = yylineno;
}

ContinueStatement::~ContinueStatement() {}


////////////////////////////////
// 
// SkipStatement class
//
////////////////////////////////

SkipStatement::SkipStatement() {
    lineno_ = yylineno;
}

SkipStatement::~SkipStatement() {}


////////////////////////////////
// 
// BinaryExpression class
//
////////////////////////////////

BinaryExpression::BinaryExpression(BinaryOperator binary_operator, 
    Expression* lhs, Expression* rhs)
  : binary_operator_(binary_operator),
    lhs_(lhs),
    rhs_(rhs) 
{
    lineno_ = yylineno;
}

BinaryExpression::~BinaryExpression() {}

BinaryOperator BinaryExpression::binary_operator() 
{
   return binary_operator_;
}

Expression* BinaryExpression::lhs() 
{
   return lhs_;
}

Expression* BinaryExpression::rhs() 
{
   return rhs_;
}


////////////////////////////////
// 
// AssignExpression class
//
////////////////////////////////

AssignExpression::AssignExpression(Expression* lhs, Expression* rhs)
  : lhs_(lhs),
    rhs_(rhs) 
{
    lineno_ = yylineno;
}

AssignExpression::~AssignExpression() {}

Expression* AssignExpression::lhs() 
{
   return lhs_;
}

Expression* AssignExpression::rhs()
{
   return rhs_;
}


////////////////////////////////
// 
// ArrayAccess class
//
////////////////////////////////

ArrayAccess::ArrayAccess(Expression* base, Expression* idx)
  : base_(base),
    idx_(idx) 
{
    lineno_ = yylineno;
}

ArrayAccess::~ArrayAccess() {}

Expression* ArrayAccess::base() 
{
   return base_;
}

Expression* ArrayAccess::idx() 
{
   return idx_;
}


////////////////////////////////
// 
// FieldAccess class
//
////////////////////////////////

FieldAccess::FieldAccess(Expression* base, char* name)
  : base_(base),
    name_(name) 
{
    lineno_ = yylineno;
}

FieldAccess::~FieldAccess() {}

Expression* FieldAccess::base() 
{
   return base_;
}

char* FieldAccess::name() 
{
   return name_;
}


////////////////////////////////
// 
// MethodInvocation class
//
////////////////////////////////

MethodInvocation::MethodInvocation(Expression* base, char* name, list<Expression*>* args)
  : base_(base),
    name_(name),
    args_(args) 
{
    lineno_ = yylineno;
}

MethodInvocation::~MethodInvocation() {}

Expression* MethodInvocation::base() 
{
   return base_;
}

char* MethodInvocation::name() 
{
   return name_;
}

list<Expression*>* MethodInvocation::args() 
{
   return args_;
}


////////////////////////////////
// 
// UnaryExpression class
//
////////////////////////////////

UnaryExpression::UnaryExpression(UnaryOperator unary_operator, Expression* arg)
  : unary_operator_(unary_operator),
    arg_(arg) 
{
    lineno_ = yylineno;
}

UnaryExpression::~UnaryExpression() {}

UnaryOperator UnaryExpression::unary_operator() 
{
   return unary_operator_;
}

Expression* UnaryExpression::arg() 
{
   return arg_;
}


////////////////////////////////
// 
// AutoExpression class
//
////////////////////////////////

AutoExpression::AutoExpression(AutoOperator auto_operator, Expression* arg)
  : auto_operator_(auto_operator),
    arg_(arg) 
{
    lineno_ = yylineno;
}

AutoExpression::~AutoExpression() {}

AutoOperator AutoExpression::auto_operator() 
{
   return auto_operator_;
}

Expression* AutoExpression::arg() 
{
   return arg_;
}


////////////////////////////////
// 
// NewArrayInstance class
//
////////////////////////////////

NewArrayInstance::NewArrayInstance(Type* type, int dimension, list<Expression*>* bounds)
  : type_(type),
    dimension_(dimension),
    bounds_(bounds) 
{
    lineno_ = yylineno;
}

NewArrayInstance::~NewArrayInstance() {}

Type* NewArrayInstance::type() 
{
   return type_;
}

int NewArrayInstance::dimension() 
{
   return dimension_;
}

list<Expression*>* NewArrayInstance::bounds() 
{
   return bounds_;
}


////////////////////////////////
// 
// NewInstance class
//
////////////////////////////////

NewInstance::NewInstance(Entity* class_entity, list<Expression*>* args)
  : class_entity_(class_entity),
    args_(args) 
{
    lineno_ = yylineno;
}

NewInstance::~NewInstance() {}

Entity* NewInstance::class_entity() 
{
   return class_entity_;
}

list<Expression*>* NewInstance::args() 
{
   return args_;
}


////////////////////////////////
// 
// ThisExpression class
//
////////////////////////////////

ThisExpression::ThisExpression() {
    lineno_ = yylineno;
}

ThisExpression::~ThisExpression() {}


////////////////////////////////
// 
// SuperExpression class
//
////////////////////////////////

SuperExpression::SuperExpression() 
{
    lineno_ = yylineno;
}

SuperExpression::~SuperExpression() {}


////////////////////////////////
// 
// IdExpression class
//
////////////////////////////////

IdExpression::IdExpression(Entity* id)
  : id_(id) 
{
    lineno_ = yylineno;
}

IdExpression::~IdExpression() {}

Entity* IdExpression::id() 
{
   return id_;
}


////////////////////////////////
// 
// NullExpression class
//
////////////////////////////////

NullExpression::NullExpression() 
{
    lineno_ = yylineno;
}

NullExpression::~NullExpression() {}


////////////////////////////////
// 
// IntegerConstant class
//
////////////////////////////////

IntegerConstant::IntegerConstant(int value)
  : value_(value) 
{
    lineno_ = yylineno;
}

IntegerConstant::~IntegerConstant() {}

int IntegerConstant::value() {
   return value_;
}


////////////////////////////////
// 
// FloatConstant class
//
////////////////////////////////

FloatConstant::FloatConstant(float value)
  : value_(value) 
{
    lineno_ = yylineno;
}

FloatConstant::~FloatConstant() {}

float FloatConstant::value() 
{
   return value_;
}


////////////////////////////////
// 
// BooleanConstant class
//
////////////////////////////////

BooleanConstant::BooleanConstant(bool value)
  : value_(value) 
{
    lineno_ = yylineno;
}

BooleanConstant::~BooleanConstant() {}

bool BooleanConstant::value() 
{
   return value_;
}


////////////////////////////////
// 
// StringConstant class
//
////////////////////////////////

StringConstant::StringConstant(char* value)
  : value_(value) 
{
    lineno_ = yylineno;
}

StringConstant::~StringConstant() {}

char* StringConstant::value() 
{
   return value_;
}


////////////////////////////////
// 
// IntType class
//
////////////////////////////////

IntType::IntType() {
  kind_ = INT_TYPE;
}

IntType::~IntType() {}


////////////////////////////////
// 
// FloatType class
//
////////////////////////////////

FloatType::FloatType() {
  kind_ = FLOAT_TYPE;
}

FloatType::~FloatType() {}


////////////////////////////////
// 
// BooleanType class
//
////////////////////////////////

BooleanType::BooleanType() {
  kind_ = BOOLEAN_TYPE;
}

BooleanType::~BooleanType() {}


////////////////////////////////
// 
// StringType class
//
////////////////////////////////

StringType::StringType() {
  kind_ = STRING_TYPE;
}

StringType::~StringType() {}


////////////////////////////////
// 
// VoidType class
//
////////////////////////////////

VoidType::VoidType() {
  kind_ = VOID_TYPE;
}

VoidType::~VoidType() {}


////////////////////////////////
// 
// ClassType class
//
////////////////////////////////

ClassType::ClassType(ClassEntity* classtype)
  : classtype_(classtype)
{
  kind_ = CLASS_TYPE;
}



// Destructor for ClassType
ClassType::~ClassType() { // Do nothing!
}

// get methods (if any) for ClassType
ClassEntity* ClassType::classtype() {
   return classtype_;
}


////////////////////////////////
// 
// InstanceType class
//
////////////////////////////////

InstanceType::InstanceType(ClassEntity* classtype)
  : classtype_(classtype) 
{
  kind_ = INSTANCE_TYPE;
}

InstanceType::~InstanceType() {}

ClassEntity* InstanceType::classtype() 
{
   return classtype_;
}


////////////////////////////////
// 
// ErrorType class
//
////////////////////////////////

ErrorType::ErrorType() {
  kind_ = ERROR_TYPE;
}

ErrorType::~ErrorType() {}


////////////////////////////////
// 
// ArrayType class
//
////////////////////////////////

ArrayType::ArrayType(Type* elementtype)
  : elementtype_(elementtype)
{
  kind_ = ARRAY_TYPE;
}

ArrayType::~ArrayType() {
}

Type* ArrayType::elementtype() {
   return elementtype_;
}


////////////////////////////////
// 
// UniverseType class
//
////////////////////////////////

UniverseType::UniverseType() {
  kind_ = UNIVERSE_TYPE;
}

UniverseType::~UniverseType() {
}


////////////////////////////////
// 
// NullType class
//
////////////////////////////////

NullType::NullType() {
  kind_ = NULL_TYPE;
}

NullType::~NullType() {
}

