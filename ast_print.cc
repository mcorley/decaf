#include <iostream>
#include "ast.h"

// Print method for IfStatement
void IfStatement::print() {
  cout << "if (";
  expr()->print();
  cout << ")" << endl;
  thenpart()->print();
  cout << "else" << endl;
  if (elsepart() == NULL)
    cout << "empty";
  else
    elsepart()->print();
}

// Print method for WhileStatement
void WhileStatement::print() {
  cout << "while (";
  expr()->print();
  cout << ")" << endl;
  body()->print();
}

// Print method for ForStatement
void ForStatement::print() {
  cout << "for (";
  if (init() != NULL)
    init()->print();
  cout << ";";
  guard()->print();
  cout << ";";
  if (update() != NULL)
    update()->print();
  cout << ")" << endl;
  body()->print();
}

// Print method for ReturnStatement
void ReturnStatement::print() {
  cout << "return ";
  if (expr() != NULL)
    expr()->print();
  cout << ";" << endl;
}

// Print method for BlockStatement
void BlockStatement::print() {
  cout << "{" << endl;
  list<Statement*>::iterator i;
  for(i=stmt_list()->begin(); i != stmt_list()->end(); i++)
    (*i)->print();
  cout << "}" << endl;
}

// Print method for ExprStatement
void ExprStatement::print() {
  expr()->print();
  cout << ";" << endl;
}

// Print method for DeclStatement
void DeclStatement::print() {
  list<Entity*>::iterator i;
  for(i=var_list()->begin(); i != var_list()->end();) {
    (*i)->print();
    i++;
    if (i != var_list()->end())
      cout << ", ";
  }
  cout << ";"<< endl;
}

// Print method for BreakStatement
void BreakStatement::print() {
  cout << "break;" << endl;
}

// Print method for ContinueStatement
void ContinueStatement::print() {
  cout << "continue;" << endl;
}

// Print method for SkipStatement
void SkipStatement::print() {
  cout << "skip;" << endl;
}

// Print method for BinaryExpression
void BinaryExpression::print() {
  switch(binary_operator()) {
    case ADD:    cout << "ADD"; break;
    case SUB:    cout << "SUB"; break;
    case MUL:    cout << "MUL"; break;
    case DIV:    cout << "DIV"; break;
    case EQ:    cout << "EQ"; break;
    case NEQ:    cout << "NEQ"; break;
    case LT:    cout << "LT"; break;
    case LEQ:    cout << "LEQ"; break;
    case GT:    cout << "GT"; break;
    case GEQ:    cout << "GEQ"; break;
    case AND:    cout << "AND"; break;
    case OR:    cout << "OR"; break;
  }
  cout << "(";
  lhs()->print();
  cout << ", ";
  rhs()->print();
  cout << ")";
}

// Print method for AssignExpression
void AssignExpression::print() {
  lhs()->print();
  cout << " = ";
  rhs()->print();
}

// Print method for ArrayAccess
void ArrayAccess::print() {
  base()->print();
  cout << "[";
  idx()->print();
  cout << "]";
}

// Print method for FieldAccess
void FieldAccess::print() {
  if (base() == NULL) {
    cout << name();
  } else {
    base()->print();
    cout << "." << name();
  }
}

// Print method for MethodInvocation
void MethodInvocation::print() {
  if (base() == NULL) {
    cout << name();
  } else {
    base()->print();
    cout << "." << name();
  }
  cout << "( ";
  list<Expression*>::iterator i;
  for(i=args()->begin(); i != args()->end();) {
    (*i)->print();
    i++;
    if (i != args()->end())
      cout << ", ";
  }
  cout << ")";
}

// Print method for UnaryExpression
void UnaryExpression::print() {
  switch(unary_operator()) {
    case NEG: cout << "NEG"; break;
    case UMINUS: cout << "UMINUS"; break;
  }
  arg()->print();
  cout << ")";
}

// Print method for AutoExpression
void AutoExpression::print() {
  switch(auto_operator()) {
    case PRE_INCR: cout << "PRE_INCR"; break;
    case POST_INCR: cout << "POST_INCR"; break;
    case PRE_DECR: cout << "PRE_DECR"; break;
    case POST_DECR: cout << "POST_DECR"; break;
  }
  arg()->print();
  cout << ")";
}

// Print method for NewArrayInstance
void NewArrayInstance::print() {
  cout << "new ";
  type()->print();
  cout << "[dimensions = " << dimension() << "]";
  list<Expression*>::iterator i;
  for(i=bounds()->begin(); i != bounds()->end(); i++) {
    cout << "[";
    (*i)->print();
    cout << "]";
  }
}

// Print method for NewInstance
void NewInstance::print() {
  cout << "new ";
  class_entity()->print_name();
  cout << "( ";
  list<Expression*>::iterator i;
  for(i=args()->begin(); i != args()->end();) {
    (*i)->print();
    i++;
    if (i != args()->end())
      cout << ", ";
  }
  cout << ")";
}

// Print method for ThisExpression
void ThisExpression::print() {
  cout << "this";
}

// Print method for SuperExpression
void SuperExpression::print() {
  cout << "super";
}

// Print method for IdExpression
void IdExpression::print() {
  if (id() != NULL)
    id()->print_name();
  else
    cout << "UndefinedID";
}

// Print method for NullExpression
void NullExpression::print() {
  cout << "null";
}

// Print method for IntegerConstant
void IntegerConstant::print() {
  cout << value();
}

// Print method for FloatConstant
void FloatConstant::print() {
  cout << value();
}

// Print method for BooleanConstant
void BooleanConstant::print() {
  cout << value();
}

// Print method for StringConstant
void StringConstant::print() {
  cout << "\"" << value() << "\"";
}

// Print method for IntType
void IntType::print() {
  cout << "int";
}

// Print method for FloatType
void FloatType::print() {
  cout << "float";
}

// Print method for BooleanType
void BooleanType::print() {
  cout << "boolean";
}

// Print method for StringType
void StringType::print() {
  cout << "string";
}

// Print method for VoidType
void VoidType::print() {
  cout << "void";
}

// Print method for ClassType
void ClassType::print() {
  classtype()->print_name();
}

// Print method for InstanceType
void InstanceType::print() {
  classtype()->print_name();
}

// Print method for ErrorType
void ErrorType::print() {
  cout << "error";
}

// Print method for ArrayType
void ArrayType::print() {
  elementtype()->print();
  cout << "[]";
}

// Print method for UniverseType
void UniverseType::print() {
  cout << "Universe";
}

// Print method for Universe
void NullType::print() {
  cout << "Null";
}

