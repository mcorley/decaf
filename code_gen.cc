#include <iostream>
#include <fstream>
#include "ast.h"
#include "error.h"

#include "statement_context.h"

extern Error *error;
extern ofstream codefile;

int static_area_offset = 0;

/** 
IMPORTANT: Add your "code", "rcode" and other methods specified in the
handout to the end of this file 
 **/

StatementContext::StatementContext() {
  loop = false;
}

StatementContext::StatementContext(StatementContext *old) {
  loop = old->loop;
  loop_iter = old->loop_iter;
  loop_exit = old->loop_exit;
}

Label current_label = 0;
Label new_label() {
  return current_label++;
}

void Program::code() {
  list<ClassEntity*>::iterator i;
  list<ClassEntity*>::iterator pClass(classes()->begin()), 
                               end(classes()->end());
  for(; pClass != end; ++pClass) {
    ClassEntity *ce = (*pClass);
    ce->code();
  }
}

void ClassEntity::code() 
{
  list<Entity*>::iterator classmbr(class_members()->begin()), 
                          end(class_members()->end());
  for (; classmbr != end; ++classmbr) {
    Entity* member = (*classmbr);
    if (member->kind() == METHOD_ENTITY)  {
      ((MethodEntity*) member)->code();
    } else if (member->kind() == CONSTRUCTOR_ENTITY) {
      ((ConstructorEntity*) member)->code();
    }
  }
}

void MethodEntity::code() {
  codefile << "entry " << this->distinct_name() << "() {" << endl;
  int nparams = this->formal_params()->size();
  if (!this->static_flag())
    nparams++;
  codefile << "new_env(" << nparams << ", " << this->locals_ << ");" << endl;
  StatementContext* c = new StatementContext();
  method_body()->code(c);
  // write out a return for methods that "fall through" the end
  codefile << "null();" << endl << "ret();" << endl;
  // the above is correct for void methods and may produce spurious results
  // for other types if those methods "fell through" the end...
  codefile << "}" << endl;
}

void ConstructorEntity::code() {
  codefile << "entry " << this->distinct_name() << "() {" << endl;
  int nparams = this->formal_params()->size() + 1;
  codefile << "new_env(" << nparams << ", " << this->locals_ << ");" << endl;
  StatementContext* c = new StatementContext();
  constructor_body()->code(c);
  // return this;
  codefile << "ildc(0);" << endl << "load();" << endl << "ret();" << endl;
  codefile << "}" << endl;
}


// Code method for NativeStatement
void NativeStatement::code(StatementContext* c) {
  codefile << this->function_name() << "();" << endl;
}


// Rcode method for StringConstant
void StringConstant::rcode() {
  // the int value is actually the index in the string table
  codefile << "ildc(" << value() << ");" << endl;
}


/*** EDIT ABOVE THIS LINE AT YOUR OWN RISK! ***/

/*** Below are stubs of methods that you need to fill in to implement the code generator **/


// Code generation method for IfStatement
void IfStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}

// Code generation method for WhileStatement
void WhileStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation method for ForStatement
void ForStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}

// Code generation method for ReturnStatement
void ReturnStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}


// Code generation method for BlockStatement
void BlockStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}

// Code generation method for ExprStatement
void ExprStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation method for DeclStatement
void DeclStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation method for BreakStatement
void BreakStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation method for ContinueStatement
void ContinueStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation method for SkipStatement
void SkipStatement::code(StatementContext*) {
  error->implementation_error("Code generation not implemented (yet)\n");
}



// Code generation (RHS)  method for BinaryExpression
void BinaryExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}



// Code generation (RHS)  method for AssignExpression
void AssignExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for ArrayAccess
void ArrayAccess::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for FieldAccess:
void FieldAccess::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

// Code generation (RHS)  method for MethodInvocation
void MethodInvocation::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}



// Code generation (RHS)  method for UnaryExpression
void UnaryExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}



// Code generation (RHS)  method for AutoExpression
void AutoExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

// Code generation (RHS)  method for NewArrayInstance: 
void NewArrayInstance::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}



// Code generation (RHS)  method for NewInstance:
void NewInstance::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for ThisExpression
void ThisExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for SuperExpression
void SuperExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

// Code generation (RHS)  method for IdExpression
void IdExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for NullExpression
void NullExpression::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for BooleanConstant
void BooleanConstant::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

// Code generation (RHS)  method for IntegerConstant
void IntegerConstant::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


// Code generation (RHS)  method for FloatConstant
void FloatConstant::rcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}



// Code generation (LHS)  method for ArrayAccess
void ArrayAccess::lcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


void ArrayAccess::load() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}


void ArrayAccess::store() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

bool ArrayAccess::indirect() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return false;
}


// Code generation (LHS)  method for FieldAccess:
void FieldAccess::lcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

void FieldAccess::load() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

void FieldAccess::store() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

bool FieldAccess::indirect() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return false;
}


// Code generation (LHS)  method for IdExpression
void IdExpression::lcode() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

void IdExpression::load() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

void IdExpression::store() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return;
}

bool IdExpression::indirect() {
  error->implementation_error("Code generation not implemented (yet)\n");
  return false;
}

