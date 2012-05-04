// AST headers

#ifndef AST_H
#define AST_H

#include <list>
#include "ast_symbols.h"

using namespace std;
using namespace __gnu_cxx;

typedef enum {
  ADD, SUB, MUL, DIV, EQ, NEQ, LT, LEQ, GT, GEQ, AND, OR
} BinaryOperator;

typedef enum {
  NEG, UMINUS
} UnaryOperator;

typedef enum {
  PRE_INCR, POST_INCR, PRE_DECR, POST_DECR
} AutoOperator;

typedef enum {
  INT_TYPE, FLOAT_TYPE, BOOLEAN_TYPE, STRING_TYPE, VOID_TYPE,
  INSTANCE_TYPE, CLASS_TYPE, ARRAY_TYPE, ERROR_TYPE, 
  UNIVERSE_TYPE, NULL_TYPE
} TypeKind;

class Statement {
  public:  
    Statement() {}
    virtual ~Statement() {}
    virtual void print() = 0;
    virtual int lineno();
    virtual void typecheck() = 0;

  protected:
    int lineno_;
};

class Expression {
  public:
    Expression() {};
    virtual ~Expression() {};
    virtual void print() = 0;
    virtual int lineno();
    virtual Type* typeinfer() = 0;
  protected:
    int lineno_;
};

class Type {
  public:
    Type() {};
    virtual ~Type() {};

    virtual TypeKind kind();
    virtual void print() = 0;
    virtual bool isSubtypeOf(Type *t);

  protected:
    TypeKind kind_;
};


////////////////////////////////
//
//     Statement classes
//
////////////////////////////////

class IfStatement : public Statement {
  public:
    IfStatement(Expression* expr, Statement* thenpart, Statement* elsepart);
    virtual ~IfStatement();

  public:
    virtual Expression* expr();
    virtual Statement* thenpart();
    virtual Statement* elsepart();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    Expression* expr_;
    Statement* thenpart_;
    Statement* elsepart_;
};

class WhileStatement : public Statement {
  public:
    WhileStatement(Expression* expr, Statement* body);
    virtual ~WhileStatement();

  public:
    virtual Expression* expr();
    virtual Statement* body();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    Expression* expr_;
    Statement* body_;
};

class ForStatement : public Statement {
  public:
    ForStatement(Statement *init, Expression *guard, Statement *update, Statement *body);
    virtual ~ForStatement();

  public:
    virtual Statement* init();
    virtual Expression* guard();
    virtual Statement* update();
    virtual Statement* body();

    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    Statement* init_;
    Expression* guard_;
    Statement* update_;
    Statement* body_;
};

class ReturnStatement : public Statement {
  public:
    ReturnStatement(Expression *expr);
    virtual ~ReturnStatement();

  public:
    virtual Expression* expr();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    Expression* expr_;
};

class BlockStatement : public Statement {
  public:
    BlockStatement(list<Statement *> *stmt_list);
    virtual ~BlockStatement();

  public:
    virtual list<Statement*>* stmt_list();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    list<Statement*>* stmt_list_;
};

class DeclStatement : public Statement {
  public:
    DeclStatement(list<Entity *> *var_list);
    virtual ~DeclStatement();

  public:
    virtual list<Entity*>* var_list();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    list<Entity*>* var_list_;
};

class ExprStatement : public Statement {
  public:
    ExprStatement(Expression* expr);
    virtual ~ExprStatement();

  public:
    virtual Expression* expr();
    void print();  // no longer pure virtual
    virtual void typecheck();

  private:
    Expression* expr_;
};

class BreakStatement : public Statement {
  public:
    BreakStatement();
    virtual ~BreakStatement();
    void print();  // no longer pure virtual
    virtual void typecheck();
};

class ContinueStatement : public Statement {
  public:
    ContinueStatement();
    virtual ~ContinueStatement();
    void print();  // no longer pure virtual
    virtual void typecheck();
};

class SkipStatement : public Statement {
  public:
    SkipStatement();
    virtual ~SkipStatement();
    void print();  // no longer pure virtual
    virtual void typecheck();
};


////////////////////////////////
//
//     Expression classes
//
////////////////////////////////

class BinaryExpression : public Expression {
  public:
    BinaryExpression(BinaryOperator binary_operator, Expression* lhs, Expression* rhs);
    virtual ~BinaryExpression();

  public:
    virtual BinaryOperator binary_operator();
    virtual Expression* lhs();
    virtual Expression* rhs();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    BinaryOperator binary_operator_;
    Expression* lhs_;
    Expression* rhs_;
};

class AssignExpression : public Expression {
  public:
    AssignExpression(Expression* lhs,Expression* rhs);
    virtual ~AssignExpression();

  public:
    virtual Expression* lhs();
    virtual Expression* rhs();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Expression* lhs_;
    Expression* rhs_;
};

class ArrayAccess : public Expression {
  public:
    ArrayAccess(Expression* base,Expression* idx);
    virtual ~ArrayAccess();

  public:
    virtual Expression* base();
    virtual Expression* idx();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Expression* base_;
    Expression* idx_;
};

class FieldAccess : public Expression {
  public:
    FieldAccess(Expression* base,char* name);
    virtual ~FieldAccess();

  public:
    virtual Expression* base();
    virtual char* name();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Expression* base_;
    char* name_;
};

class MethodInvocation : public Expression {
  public:
    MethodInvocation(Expression* base, char* name, list<Expression*>* args);
    virtual ~MethodInvocation();

  public:
    virtual Expression* base();
    virtual char* name();
    virtual list<Expression*>* args();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Expression* base_;
    char* name_;
    list<Expression*>* args_;
};

class UnaryExpression : public Expression {
  public:
    UnaryExpression(UnaryOperator unary_operator, Expression* arg);
    virtual ~UnaryExpression();

  public:
    virtual UnaryOperator unary_operator();
    virtual Expression* arg();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    UnaryOperator unary_operator_;
    Expression* arg_;
};

class AutoExpression : public Expression {
  public:
    AutoExpression(AutoOperator auto_operator, Expression* arg);
    virtual ~AutoExpression();

  public:
    virtual AutoOperator auto_operator();
    virtual Expression* arg();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    AutoOperator auto_operator_;
    Expression* arg_;
};

class NewArrayInstance : public Expression {
  public:
    NewArrayInstance(Type *type, int dimension, list<Expression *> *bounds);
    virtual ~NewArrayInstance();

  public:
    virtual Type* type();
    virtual int dimension();
    virtual list<Expression*>* bounds();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Type* type_;
    int dimension_;
    list<Expression*>* bounds_;
};

class NewInstance : public Expression {
  public:
    NewInstance(Entity *class_entity, list<Expression *> *args);
    virtual ~NewInstance();

  public:
    virtual Entity* class_entity();
    virtual list<Expression*>* args();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Entity* class_entity_;
    list<Expression*>* args_;
};

class ThisExpression : public Expression {
  public:
    ThisExpression();
    virtual ~ThisExpression();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();
};

class SuperExpression:public Expression {
  public:
    SuperExpression();
    virtual ~SuperExpression();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();
};

class IdExpression : public Expression {
  public:
    IdExpression(Entity *id);
    virtual ~IdExpression();

  public:
    virtual Entity* id();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    Entity* id_;
};

class NullExpression : public Expression {
  public:
    NullExpression();
    virtual ~NullExpression();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();
};

class IntegerConstant : public Expression {
  public:
    IntegerConstant(int value);
    virtual ~IntegerConstant();

  public:
    virtual int value();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    int value_;
};

class FloatConstant : public Expression {
  public:
    FloatConstant(float value);
    virtual ~FloatConstant();

  public:
    virtual float value();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    float value_;
};

class BooleanConstant : public Expression {
  public:
    BooleanConstant(bool value);
    virtual ~BooleanConstant();

  public:
    virtual bool value();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    bool value_;
};

class StringConstant : public Expression {
  public:
    StringConstant(char* value);
    virtual ~StringConstant();

  public:
    virtual char* value();
    void print();  // no longer pure virtual
    virtual Type* typeinfer();

  private:
    char* value_;
};


////////////////////////////////
//
//       Type classes
//
////////////////////////////////

class IntType : public Type {
  public:
    IntType();
    virtual ~IntType();
    void print();  // no longer pure virtual
};

class FloatType : public Type {
  public:
    FloatType();
    virtual ~FloatType();
    void print();  // no longer pure virtual
};

class BooleanType : public Type {
  public:
    BooleanType();
    virtual ~BooleanType();
    void print();  // no longer pure virtual
};

class StringType : public Type {
  public:
    StringType();
    virtual ~StringType();
    void print();  // no longer pure virtual
};

class VoidType : public Type {
  public:
    VoidType();
    virtual ~VoidType();
    void print();  // no longer pure virtual
};

class ClassType : public Type {
  public:
    ClassType(ClassEntity* classtype);
    virtual ~ClassType();

  public:
    virtual ClassEntity* classtype();
    void print();  // no longer pure virtual
  private:
    ClassEntity *classtype_;
};

class InstanceType : public Type {
  public:
    InstanceType(ClassEntity* classtype);
    virtual ~InstanceType();

  public:
    virtual ClassEntity* classtype();
    void print();  // no longer pure virtual

  private:
    ClassEntity* classtype_;
};

class ErrorType : public Type {
  public:
    ErrorType();
    virtual ~ErrorType();
    void print();  // no longer pure virtual
};

class ArrayType : public Type {
  public:
    ArrayType(Type* elementtype);
    virtual ~ArrayType();

  public:
    virtual Type* elementtype();
    void print();  // no longer pure virtual

  private:
    Type* elementtype_;
};

// UniverseType is the top-most in type hierarchy; 
// every type is in UniverseType
class UniverseType : public Type {
  public:
    UniverseType();
    virtual ~UniverseType();
    void print();  // no longer pure virtual
};

// NullType is a bottom in type hierarchy; 
// it is in every class type 
class NullType : public Type {
  public:
    NullType();
    virtual ~NullType();
    void print();  // no longer pure virtual
};

#endif // AST_H

