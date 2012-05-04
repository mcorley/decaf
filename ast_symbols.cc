// AstSymbols.cc

#include <cstdio>
#include <cstring>
#include <iostream>
#include "ast_symbols.h"
#include "ast.h"
using namespace std;

extern EntityTable *global_symtab;

////////////////////////////////
//
// Class Entitites
//
/////////////////////////////////

ClassEntity::ClassEntity(const char* name, ClassEntity *superclass, list<Entity*> *class_members)
  : Entity::Entity(name, CLASS_ENTITY),
    superclass_(superclass),
    class_members_(class_members) 
{
    global_symtab->add_entity(this);
}

ClassEntity::~ClassEntity() {}

ClassEntity* ClassEntity::superclass() {
  return superclass_;
}

list<Entity*> *ClassEntity::class_members() {
  return class_members_;
}

void ClassEntity::set_class_members(list<Entity*> *class_members) {
  class_members_ = class_members;
}

void ClassEntity::add_class_member(Entity* new_member) {
  class_members_->push_back(new_member);
}

void ClassEntity::print() 
{
  cout << "class ";
  print_name();
  if (superclass() != NULL) {
    cout << " extends ";
    superclass()->print_name();
  }
  cout << "{" << endl;
  list<Entity*>::iterator i;
  cout << "// Has " << class_members()->size() << " members" << endl;
  for(i=class_members()->begin(); i != class_members()->end(); i++) {
    (*i)->print();
    cout << endl;
  }
  cout << "}" << endl;
}


////////////////////////////////
//
// Method Entities
//
/////////////////////////////////

MethodEntity::MethodEntity(const char* name, bool visibility_flag, bool static_flag, 
    Type *return_type, list <Entity*> *formal_params, Statement *method_body)
  : Entity::Entity(name, METHOD_ENTITY),
    visibility_flag_(visibility_flag),
    static_flag_(static_flag),
    return_type_(return_type),
    formal_params_(formal_params),
    method_body_(method_body)
{
    global_symtab->add_entity(this);
}

MethodEntity::~MethodEntity() {}

bool MethodEntity::visibility_flag() {
  return visibility_flag_;
}

bool MethodEntity::static_flag() {
  return static_flag_;
}

Type *MethodEntity::return_type() {
  return return_type_;
}

list <Entity*> *MethodEntity::formal_params() {
  return formal_params_;
}

Statement* MethodEntity::method_body() {
  return method_body_;
}

void MethodEntity::set_visibility_flag(bool visibility_flag) {
  visibility_flag_ = visibility_flag;
}

void MethodEntity::set_static_flag(bool static_flag) {
  static_flag_ = static_flag;
}

void MethodEntity::set_return_type(Type *return_type) {
  return_type_ = return_type;
}

void MethodEntity::set_formal_params(list<Entity*> *formal_params) {
  formal_params_ = formal_params;
}

void MethodEntity::set_method_body(Statement *method_body) {
  method_body_ = method_body;
}

void MethodEntity::print() 
{
  //  cout << "method: ";
  if (visibility_flag())
    cout << "public ";
  else
    cout << "private ";
  if (static_flag())
    cout << "static ";
  return_type()->print();
  cout << " ";
  print_name();
  cout << "( ";
  list<Entity*>::iterator i;
  for(i=formal_params()->begin(); i != formal_params()->end(); i++) {
    (*i)->print();
    cout << ",";
  }
  cout << ")" << endl;
  method_body()->print();
}


////////////////////////////////
//
// Field Entities
//
/////////////////////////////////

FieldEntity::FieldEntity(const char *name, bool visibility_flag, 
    bool static_flag, Type *type, int dimensions)
  : Entity::Entity(name, FIELD_ENTITY),
    visibility_flag_(visibility_flag),
    static_flag_(static_flag),
    type_(type),
    dimensions_(dimensions)
{
  global_symtab->add_entity(this);
}

FieldEntity::~FieldEntity() {}

bool FieldEntity::visibility_flag() {
  return visibility_flag_;
}

bool FieldEntity::static_flag() {
  return static_flag_;
}

Type* FieldEntity::type() {
  return type_;
}

int FieldEntity::dimensions() {
  return dimensions_;
}

void FieldEntity::print() 
{
  //  cout << "field: ";
  if (visibility_flag())
    cout << "public ";
  else
    cout << "private ";
  if (static_flag())
    cout << "static ";
  type()->print();
  cout << " ";
  print_name();
  if (dimensions() > 0) {
    cout << "[dimensions = " << dimensions() << "]";
  }
  cout << ";" << endl;
}


////////////////////////////////
//
// Constructor Entities
//
/////////////////////////////////

ConstructorEntity::ConstructorEntity(const char* name, bool visibility_flag, 
    list <Entity*> *formal_params, Statement *constructor_body)
  : Entity(name, CONSTRUCTOR_ENTITY),
    visibility_flag_(visibility_flag),
    formal_params_(formal_params),
    constructor_body_(constructor_body)
{
  global_symtab->add_entity(this);
}

ConstructorEntity::~ConstructorEntity() {}

bool ConstructorEntity::visibility_flag() {
  return visibility_flag_;
}

list <Entity*> *ConstructorEntity::formal_params() {
  return formal_params_;
}

Statement* ConstructorEntity::constructor_body() {
  return constructor_body_;
}

void ConstructorEntity::set_visibility_flag(bool visibility_flag) {
  visibility_flag_ = visibility_flag;
}

void ConstructorEntity::set_formal_params(list<Entity*> *formal_params) {
  formal_params_ = formal_params;
}

void ConstructorEntity::set_constructor_body(Statement *constructor_body) {
  constructor_body_ = constructor_body;
}

/*
   ostream& ConstructorEntity::operator<<(ostream& out, const ConstructorEntity& ce) {
   out << "constructor: ";
   if (ce.visibility_flag())
   out << "public ";
   out << Entity::toString();
   out << "( ";
   list<Entity*>::iterator i;
   for(i=formal_params()->begin(); i != formal_params()->end(); i++) {
   out << (*i)->print() << ", ";
   }
   out << ")" << endl;
   out << constructor_body();
   return out;
   }
   */

void ConstructorEntity::print() 
{
  cout << "constructor: ";
  if (visibility_flag())
    cout << "public ";
  else
    cout << "private ";
  print_name();
  cout << "( ";
  list<Entity*>::iterator i;
  for(i=formal_params()->begin(); i != formal_params()->end(); i++) {
    (*i)->print();
    cout << ", ";
  }
  cout << ")" << endl;
  constructor_body()->print();
}


////////////////////////////////
//
// Variable Entities
//
/////////////////////////////////

VariableEntity::VariableEntity(const char *name, Type *type, int dimensions)
  : Entity::Entity(name, VARIABLE_ENTITY),
    type_(type),
    dimensions_(dimensions)
{
  global_symtab->add_entity(this);
}

VariableEntity::~VariableEntity() {}

Type* VariableEntity::type() {
  return type_;
}

int VariableEntity::dimensions() {
  return dimensions_;
}

void VariableEntity::set_type(Type *type) {
  type_ = type;
}

void VariableEntity::print() 
{
  //  cout << "variable: ";
  type()->print();
  cout << " ";
  print_name();
  if (dimensions() > 0) {
    cout << "[dimensions = " << dimensions() << "]";
  }
}

