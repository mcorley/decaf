#include <iostream>
#include "ast.h"
#include "error.h"

extern Error *error;

int maxoffset;


void Program::allocate() {
  list<ClassEntity*>::iterator i;
  for(i=classes()->begin(); i != classes()->end(); i++)
    (*i)->allocate();
}

void ClassEntity::allocate() {

  list<Entity*>::iterator i;

  int static_offset = 0;
  int instance_offset = 0;

  if (this->superclass() != NULL) {
    static_offset = this->superclass()->class_size();
    instance_offset = this->superclass()->instance_size();
  }

  // set the offsets for each field;
  for(i=class_members()->begin(); i != class_members()->end(); i++) {
    if ((*i)->kind() == FIELD_ENTITY)  {
      FieldEntity* f = (FieldEntity*) (*i);
      if (f->static_flag())
        f->set_offset(static_offset++);
      else
        f->set_offset(instance_offset++);
    } else if ((*i)->kind() == METHOD_ENTITY) {
      ((MethodEntity*)(*i))->allocate();
    } else if ((*i)->kind() == CONSTRUCTOR_ENTITY) {
      ((ConstructorEntity*)(*i))->allocate();
    }
  }

  this->instance_size_ = instance_offset;
  this->class_size_ = static_offset;

}

void MethodEntity::allocate() {
  int offset;
  if (this->static_flag())
    offset = 0;
  else
    offset = 1;

  list<VariableEntity*>::iterator i;

  // give offsets to all parameters first:
  for(i=formal_params()->begin(); i != formal_params()->end(); i++) {
    (*i)->set_offset(offset++);
  }

  maxoffset = offset;
  // give offsets to all other local variables:
  method_body()->allocate(offset);
  this->locals_ = maxoffset - offset;
}

void ConstructorEntity::allocate() {
  int offset = 1; // note: constructors are non static, so have a this parameter
  list<VariableEntity*>::iterator i;

  // give offsets to all parameters first:
  for(i=formal_params()->begin(); i != formal_params()->end(); i++) {
    (*i)->set_offset(offset++);
  }

  maxoffset = offset;
  // give offsets to all other local variables:
  constructor_body()->allocate(offset);
  this->locals_ = maxoffset - offset;
}


// Allocate method for IfStatement
int IfStatement::allocate(int offset) {
  thenpart()->allocate(offset);
  elsepart()->allocate(offset);
  return offset;
}



// Allocate method for WhileStatement
int WhileStatement::allocate(int offset) {
  body()->allocate(offset);
  return offset;
}



// Allocate method for ForStatement
int ForStatement::allocate(int offset) {
  init()->allocate(offset); // not needed, actually, since init and update
  update()->allocate(offset);  // cannot declare new vars
  body()->allocate(offset);
  return offset;
}



// Allocate method for ReturnStatement
int ReturnStatement::allocate(int offset) {
  return offset;
}



// Allocate method for BlockStatement
int BlockStatement::allocate(int init_offset) {
  int offset = init_offset;
  list<Statement*>::iterator i;
  for(i=stmt_list()->begin(); i != stmt_list()->end(); i++)
    offset = (*i)->allocate(offset);
  return init_offset;  // all allocated vars are local, so init_offset is unchanged.
}

// Allocate method for ExprStatement
int ExprStatement::allocate(int offset) {
  return offset;
}

// Allocate method for DeclStatement
int DeclStatement::allocate(int init_offset) {
  list<VariableEntity*>::iterator i;
  int offset = init_offset;
  for(i=var_list()->begin(); i != var_list()->end(); i++)
    (*i)->set_offset(offset++);
  maxoffset = (offset > maxoffset)? offset: maxoffset;
  return offset;
}


// Allocate method for BreakStatement TO BE DONE
int BreakStatement::allocate(int offset) {
  return offset;
}



// Allocate method for ContinueStatement TO BE DONE
int ContinueStatement::allocate(int offset) {
  return offset;
}


// Allocate method for SkipStatement
int SkipStatement::allocate(int offset) {
  return offset;
}

// Allocate method for NativeStatement
int NativeStatement::allocate(int offset) {
  return offset;
}


