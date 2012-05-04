#include <iostream>
#include "ast.h"

//
// We have a "top-level" type:
//  UniverseType, which is the top-most type; everything, including error, 
//     is in universe.
// Moreover, we have a "top-level" class:
//   Object, which is the top-most class; every class is derived from Object.
//   We'll have to ensure that the Object class is in the symbol table
//   initially.
//

bool Type::isSubtypeOf(Type *t) {
  // Is this type a subtype of an arbitrary type t?
  if ((this->kind() == ERROR_TYPE) || (t->kind() == ERROR_TYPE))
    return true; // avoid cascading errors
  if (t->kind() == UNIVERSE_TYPE) //everything is a subtype of universe
    return true;
  if ((this->kind() == NULL_TYPE) // Null type is a subtype of any instance type
      && (t->kind() == INSTANCE_TYPE))
    return true;

  switch(this->kind()){
    case CLASS_TYPE:
      if (t->kind() != CLASS_TYPE)
        return false;
      {
        ClassEntity* e1 = ((ClassType*)this)->classtype();
        ClassEntity* e2 = ((ClassType*)t)->classtype();
        while(e1 != NULL && e1!=e2)
          e1 = e1->superclass();
        return   e1!=NULL;
      }
      break;

    case INSTANCE_TYPE:
      if (t->kind() != INSTANCE_TYPE)
        return false;
      {
        ClassEntity* e1 = ((InstanceType*)this)->classtype();
        ClassEntity* e2 = ((InstanceType*)t)->classtype();
        while(e1 != NULL && e1!=e2)
          e1 = e1->superclass();
        return   e1!=NULL;
      }
      break;

    case ARRAY_TYPE:
      if (t->kind() != ARRAY_TYPE)
        return false;
      return (((ArrayType*)this)->elementtype())->isSubtypeOf(
          ((ArrayType*)t)->elementtype());
      break;

    default:
      // for all other types, equality and subtypeness coincide:
      return this->kind() == t->kind();
  }
  return false;
}

TypeKind Type::kind() {
  return kind_;
}
