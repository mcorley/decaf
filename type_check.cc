#include <iostream>
#include "ast.h"
#include "error.h"

// Useful for printing error messages.
extern Error *error;

// The top-most class; every class is derived from objectclass.
extern ClassEntity *objectclass;

// Type pointers used for checking subtypes.
ArrayType *array_type;
BooleanType *bool_type;
ClassType *class_type;
ErrorType *error_type;
FloatType *float_type;
InstanceType *instance_type;
IntType *integer_type;
NullType *null_type;
StringType *string_type;
UniverseType *universe_type;
VoidType *void_type;

// Maintains some state inside the block of code 
// currently being type-checked. 
struct ScopeState { 
  ClassEntity *class_in_scope; // A pointer to the class in scope.
  Kind kind_in_scope; // The kind of the current entity in maintaining current scope.
  bool method_static_flag; // True if the method in scope is STATIC.  
  bool method_visibility_flag; // True indicates the method in scope is PUBLIC. 
  Type *method_return_type; // A pointer to the return type of the method in scope.   
} current_scope;

void initialize_typechecker() 
{
  bool_type = new BooleanType();
  error_type = new ErrorType();
  float_type = new FloatType();
  integer_type = new IntType();
  null_type = new NullType();
  string_type = new StringType();
  universe_type = new UniverseType();
  void_type = new VoidType();

  // Initial state of the type checker.
  current_scope.class_in_scope = objectclass;
  current_scope.kind_in_scope = CLASS_ENTITY;
  current_scope.method_static_flag = false;
  current_scope.method_visibility_flag = false;
  current_scope.method_return_type = error_type;
}

// Type Checking Functions
// All Statement classes, as well as all ClassEntity, MethodEntity 
// and ConstructorEntity classes, implement a typecheck() method.
// This method checks the type correctness of a statement.

void ClassEntity::typecheck() 
{
  // Update this class to be the current class in scope.
  current_scope.class_in_scope = this;

  // A class entity is type checked when every one of its method 
  // and constructor members are type checked.  
  list<Entity *>::iterator classmbr(this->class_members()->begin()),
                           end(this->class_members()->end());
  for (; classmbr != end; ++classmbr) {
    if ((*classmbr)->kind() == METHOD_ENTITY) {
      MethodEntity *me = dynamic_cast<MethodEntity *>((*classmbr));
      me->typecheck();
    } else if ((*classmbr)->kind() == CONSTRUCTOR_ENTITY) {
      ConstructorEntity *ce = dynamic_cast<ConstructorEntity *>((*classmbr));
      ce->typecheck();
    }
  }
}


void MethodEntity::typecheck() 
{
  // Update this method to be the current subroutine in scope.
  current_scope.kind_in_scope = METHOD_ENTITY;
  current_scope.method_return_type = this->return_type();
  current_scope.method_static_flag = this->static_flag();
  current_scope.method_visibility_flag = this->visibility_flag();

  // A method entity is type checked when its body is type checked.
  this->method_body()->typecheck();
}


void ConstructorEntity::typecheck() 
{
  // Update this constructor to be the current subroutine in scope.
  current_scope.kind_in_scope = CONSTRUCTOR_ENTITY;
  current_scope.method_return_type = error_type; // NO RETURN TYPE
  current_scope.method_static_flag = false; // NON-STATIC
  current_scope.method_visibility_flag = this->visibility_flag();

  // A constructor entity is type checked when its body is type checked.
  this->constructor_body()->typecheck();
}


// Typecheck method for IfStatement
// The expression part must have a boolean type and
// the then and else parts must be type correct.
void IfStatement::typecheck() 
{
  Type *condition_type = this->expr()->typeinfer();
  if (!(condition_type->isSubtypeOf(bool_type))) {
    string errmsg("error: expected boolean type in if-statement condition\n");
    error->type_error(this->lineno(), errmsg, condition_type);
  }
  this->thenpart()->typecheck();
  this->elsepart()->typecheck();
}


// Typecheck method for WhileStatement
// The expression part must have a boolean type and
// the body must be type correct.
void WhileStatement::typecheck() 
{
  Type *condition_type = this->expr()->typeinfer();
  if (!(condition_type->isSubtypeOf(bool_type))) {
    string errmsg("error: expected boolean type in while-statement condition\n");
    error->type_error(this->lineno(), errmsg, condition_type);
  }
  this->body()->typecheck();
}


// Typecheck method for ForStatement
// The guard must have a boolean type and the other parts, 
// init, update, and body, must be type correct.
void ForStatement::typecheck() 
{  
  Type *guard_type = this->guard()->typeinfer();
  if (!(guard_type->isSubtypeOf(bool_type))) {
    string errmsg("error: expected boolean type in for-statement guard\n");
    error->type_error(this->lineno(), errmsg, guard_type);
  }
  this->init()->typecheck();
  this->update()->typecheck();
  this->body()->typecheck();
}


// Typecheck method for ReturnStatement
// The expression type must be a subtype of the current 
// method’s return type. A return statement is valid only 
// when it occurs in a method (not a constructor).
void ReturnStatement::typecheck() 
{
  // ONLY METHODS can have a return type.
  if (current_scope.kind_in_scope != METHOD_ENTITY) {
    string errmsg("error: return statement in constructor not permitted\n");
    error->syntax_error(this->lineno(), errmsg);
  }

  // If the expr part of the return statement is omitted then 
  // the return type of the method is void.
  Type *type = (this->expr()) ? this->expr()->typeinfer() : void_type;

  // Make sure the return type is valid.
  if (!(type->isSubtypeOf(current_scope.method_return_type))) {
    string errmsg("error: invalid return type\n");
    error->type_error(this->lineno(), errmsg, 
        current_scope.method_return_type, type);
  }
}


// Typecheck method for BlockStatement
// Each statement in the block must be type correct.
void BlockStatement::typecheck() 
{
  list<Statement *>::iterator stmt(this->stmt_list()->begin()),
                              end(this->stmt_list()->begin());
  for (; stmt != end; ++stmt)
    (*stmt)->typecheck();
}


// Typecheck method for ExprStatement
// The expression must be type correct (the type of the 
// expression itself will be ignored).
void ExprStatement::typecheck() {
  this->expr()->typeinfer();
}


// These statements have no type checking done on them.
void DeclStatement::typecheck() {}
void BreakStatement::typecheck() {}
void ContinueStatement::typecheck() {}
void SkipStatement::typecheck() {}




// Type Inference Functions
// All Expressions classes (BinaryExpression, AssignExpression,...)
// implement a typeinfer() method. This method computes the type of 
// an expression’s result, based on the types of the expression’s 
// arguments.

// Typeinfer method for BinaryExpression
Type* BinaryExpression::typeinfer() 
{
  Type *lhs_type = this->lhs()->typeinfer();
  Type *rhs_type = this->rhs()->typeinfer();

  switch (binary_operator()) {
    case ADD:
    case SUB:
    case MUL:
    case DIV:
      // Both arguments should be numeric 
      // (i.e. integers or floats).
      if (!(lhs_type->isSubtypeOf(integer_type)) && 
          !(lhs_type->isSubtypeOf(float_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, lhs_type);
        return error_type;
      }

      if (!(rhs_type->isSubtypeOf(integer_type)) && 
          !(rhs_type->isSubtypeOf(float_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, rhs_type);
        return error_type;
      }

      // The result is an integer if both arguments are int.
      // Else, the result is a float
      if (lhs_type->isSubtypeOf(integer_type) && 
          rhs_type->isSubtypeOf(integer_type))
        return integer_type;
      return float_type;

    case LT:
    case LEQ:
    case GT:
    case GEQ:
      // Both arguments should be numeric 
      // (i.e. integers or floats).
      if (!(lhs_type->isSubtypeOf(integer_type)) && 
          !(lhs_type->isSubtypeOf(float_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, lhs_type);
        return error_type;
      }

      if (!(rhs_type->isSubtypeOf(integer_type)) && 
          !(rhs_type->isSubtypeOf(float_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, rhs_type);
        return error_type;
      }
      // The result is a boolean.
      return bool_type;

    case EQ:
    case NEQ:
      // The two arguments should be compatible
      // i.e one should be a subtype of another.
      if (!(lhs_type->isSubtypeOf(rhs_type)) && 
          !(rhs_type->isSubtypeOf(lhs_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, lhs_type, rhs_type);
        return error_type;
      }
      // The result is a boolean
      return bool_type;

    case AND:
    case OR:
      // The two arguments should be boolean. 
      if (!(lhs_type->isSubtypeOf(bool_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, lhs_type);
        return error_type;
      } 

      if (!(rhs_type->isSubtypeOf(bool_type))) {
        string errmsg("error: invalid operand type to binary expression\n");
        error->type_error(this->lineno(), errmsg, rhs_type);
        return error_type;
      }
      // The result is a boolean.
      return bool_type; 

    default:
      string errmsg("error: invalid operand types to binary expression\n");
      error->type_error(this->lineno(), errmsg, lhs_type, rhs_type);
      return error_type;
  }
}


// Typeinfer method for AssignExpression
Type* AssignExpression::typeinfer() 
{
  Type *lhs_type = this->lhs()->typeinfer();
  Type *rhs_type = this->rhs()->typeinfer();

  // The right hand side should be a subtype of the left hand side.
  if (!(rhs_type->isSubtypeOf(lhs_type))) {
    string errmsg("error: invalid operand types to assign expression\n");
    error->type_error(this->lineno(), errmsg, lhs_type, rhs_type);
    return error_type;
  }
  return rhs_type;
}


// Typeinfer method for ArrayAccess
Type* ArrayAccess::typeinfer() 
{
  Type *base_type = this->base()->typeinfer();
  Type *index_type = this->idx()->typeinfer();

  // The base of array access should be an array type.
  if (!(base_type->isSubtypeOf(array_type))) {
    string errmsg("error: expected an array type for base of array access\n");
    error->type_error(this->lineno(), errmsg, base_type);
    return error_type;
  }

  // The index should be an integer.  
  if (!(index_type->isSubtypeOf(integer_type))) {
    string errmsg("error: expected an integer type for index of array access\n");
    error->type_error(this->lineno(), errmsg, index_type);
    return error_type;
  }

  // The result is the same as the base type.
  return base_type;
}


// Typeinfer method for FieldAccess:
// A field access is of the form b.x where b is an expression
// called the base, and x is a field name.
Type* FieldAccess::typeinfer() 
{
  // There are two cases we need to consider: 
  // 1.) Accessing NON-STATIC fields. 
  //     This is indicated by a base expression with type kind INSTANCE_TYPE.     
  // 2.) Accessing STATIC fields.
  //     This is indicated by a base expression with type kind CLASS_TYPE. 
  bool field_static_flag;
  ClassEntity *baseclass;
  Type *base_type = this->base()->typeinfer();
  if (base_type->kind() == INSTANCE_TYPE) {
    field_static_flag = false;
    InstanceType *instance_type = dynamic_cast<InstanceType *>(base_type);
    baseclass = instance_type->classtype();
  } else if (base_type->kind() == CLASS_TYPE) {
    field_static_flag = true;
    ClassType *class_type = dynamic_cast<ClassType *>(base_type);
    baseclass = class_type->classtype();
  } else { 
    string errmsg("error: expected instance or class type in base expression\n");
    error->type_error(this->lineno(), errmsg, base_type);
    return error_type;
  }

  // Worst case, we search up the entire class hierarchy.
  for (; baseclass; baseclass = baseclass->superclass()) {
    // Search through the current base class for the matching field.
    list<Entity *>::iterator classmbr(baseclass->class_members()->begin()),
                             end(baseclass->class_members()->end());
    for (; classmbr != end; ++classmbr) {
      if ((*classmbr)->kind() != FIELD_ENTITY) 
        continue;
      FieldEntity *fe = dynamic_cast<FieldEntity *>((*classmbr));
      if (strcmp(this->name(), fe->name()) != 0)
        continue;

      // Check for correct modifiers,
      // i.e. correct static access and that the field is PUBLIC.
      if (field_static_flag == fe->static_flag() && fe->visibility_flag())
        return fe->type();

      // If we get here, there was field defined with this name, but incorrect
      // static access or not defined PUBLIC, so access to it is not allowed.
      string errmsg("error: illegal access to field\n");
      error->type_error(this->lineno(), errmsg, this->name());
      return error_type;
    }
  }
  // If we get here there was no field defined with 
  // this name in the entire hierarchy.
  string errmsg("error: field not defined\n");
  error->type_error(this->lineno(), errmsg, this->name());
  return error_type;
}


// Typeinfer method for MethodInvocation
Type* MethodInvocation::typeinfer() 
{
  // There are two cases we need to consider: 
  // 1.) Accessing NON-STATIC methods. 
  //     This is indicated by a base expression with type kind INSTANCE_TYPE.     
  // 2.) Accessing STATIC methods.
  //     This is indicated by a base expression with type kind CLASS_TYPE.
  bool method_static_flag;
  ClassEntity *baseclass;
  Type *base_type = this->base()->typeinfer();
  if (base_type->kind() == INSTANCE_TYPE) {
    method_static_flag = false;
    InstanceType *instance_type = dynamic_cast<InstanceType *>(base_type);
    baseclass = instance_type->classtype();
  } else if (base_type->kind() == CLASS_TYPE) {
    method_static_flag = true;
    ClassType *class_type = dynamic_cast<ClassType *>(base_type);
    baseclass = class_type->classtype();
  } else { 
    string errmsg("error: expected instance or class type in base expression\n");
    error->type_error(this->lineno(), errmsg, base_type);
    return error_type;
  }

  // Infer types of the actual arguments passed for this invocation.
  list<Type *> *arg_types = new list<Type *>();
  list<Expression *>::iterator arg(this->args()->begin()), 
                               end(this->args()->end());
  for (; arg != end; ++arg)
    arg_types->push_back((*arg)->typeinfer());  

  // Worst case, we search up the entire class hierarchy.
  for (; baseclass; baseclass = baseclass->superclass()) {
    list<Entity *>::iterator classmbr(baseclass->class_members()->begin()),
                             end(baseclass->class_members()->end());
    for (; classmbr != end; ++classmbr) {
      // If classmbr is not a method labeled by this method name, move on.
      if ((*classmbr)->kind() != METHOD_ENTITY) 
        continue;
      MethodEntity *me = dynamic_cast<MethodEntity *>((*classmbr));
      if (strcmp(this->name(), me->name()) != 0)
        continue;
     
      // --CHECK ARGUMENT TYPES WITH PARAMETER TYPES--
      // The types of the actual arguments passed for this invocation should
      // be subtypes of the corresponding parameter types expected.
      list<Type *>::iterator arg_type(arg_types->begin());
      list<Entity *>::iterator param(me->formal_params()->begin()),
                               end(me->formal_params()->end());
      for (; param != end; ++param, ++arg_type) {
        if ((*param)->kind() != VARIABLE_ENTITY) {
          string errmsg("error: expected variable for formal parameter\n");
          error->type_error(this->lineno(), errmsg, (*param));
          return error_type;
        }

        VariableEntity *ve = dynamic_cast<VariableEntity *>((*param));
        if (!(*arg_type)->isSubtypeOf(ve->type())) {
          string errmsg("error: actual arguments don't match formal parameters\n");
          error->type_error(this->lineno(), errmsg, (*arg_type), ve->type());
          return error_type;
        }
      }  

      // Check for correct modifiers,
      // i.e. correct static access and that the field is PUBLIC.
      if (method_static_flag == me->static_flag() && me->visibility_flag())
        return me->return_type();

      // If we get here, there is a method defined with this name, but incorrect
      // static access or not defined PUBLIC, so access to it is not allowed.
      string errmsg("error: illegal access to method\n");
      error->type_error(this->lineno(), errmsg, this->name());
      return error_type;
    }
  }
  // If we get here there was no method defined with 
  // this name in the entire hierarchy.
  string errmsg("error: method not defined\n");
  error->type_error(this->lineno(), errmsg, this->name());
  return error_type;
}


// Typeinfer method for UnaryExpression
Type* UnaryExpression::typeinfer() 
{
  Type *arg_type = this->arg()->typeinfer();
  switch (unary_operator()) {
    case NEG:
      // The argument should be boolean.
      if (!(arg_type->isSubtypeOf(bool_type))) {
        string errmsg("error: invalid operand type to unary neg expression\n");
        error->type_error(this->lineno(), errmsg, arg_type);
        return error_type;
      }
      return bool_type;
      
    case UMINUS:
      // The argument should be numeric.
      if (!(arg_type->isSubtypeOf(integer_type)) || 
          !(arg_type->isSubtypeOf(float_type))) {
        string errmsg("error: invalid operand type to unary minus expression\n");
        error->type_error(this->lineno(), errmsg, arg_type);
        return error_type;
      }
      return arg_type;
      
    default:
      string errmsg("error: invalid operand type to unary expression\n");
      error->type_error(this->lineno(), errmsg, arg_type);
      return error_type;
  }
}


// Typeinfer method for AutoExpression
Type* AutoExpression::typeinfer() 
{
  // The argument should be an integer.
  Type *arg_type = this->arg()->typeinfer();
  if (!(arg_type->isSubtypeOf(integer_type))) {
    string errmsg("error: invalid operand type to auto expression\n");
    error->type_error(this->lineno(), errmsg, arg_type);
    return error_type;
  }
  return arg_type;
}


// Typeinfer method for NewArrayInstance: 
Type* NewArrayInstance::typeinfer() 
{
  // For an one-dimensional array, the type field gives the element type 
  // of the array (say \alpha). Then the result of new array instance 
  // creation is of type ArrayType(\alpha).
  const int dim = this->dimension();
  if (dim == 1)
    return new ArrayType(this->type());

  // When an n-dimensional array with element type \alpha is created, the 
  // result type is ArrayType(ArrayType(···<ntimes> ArrayType(\alpha)···)).
  Type *ret_type = this->type();
  for (int i = 0; i < dim; ++i)
    ret_type = new ArrayType(ret_type);
  return ret_type;
}


// Typeinfer method for NewInstance
Type* NewInstance::typeinfer() 
{
  // Infer types of the actual arguments passed for this invocation.
  list<Type *> *arg_types = new list<Type *>();
  list<Expression *>::iterator arg(this->args()->begin()),
                               arg_end(this->args()->end());
  for (; arg != arg_end; ++arg)
    arg_types->push_back((*arg)->typeinfer());

  // Only constructors in the given class (not super classes) are considered.
  ClassEntity *thisclass = dynamic_cast<ClassEntity *>(this->class_entity());
  list<Entity *>::iterator classmbr(thisclass->class_members()->begin()),
                           classmbr_end(thisclass->class_members()->end());
  for (; classmbr != classmbr_end; ++classmbr) {
    if ((*classmbr)->kind() != CONSTRUCTOR_ENTITY)
      continue;    

    // The types of the actual arguments passed for this invocation should
    // be subtypes of the corresponding parameter types expected.
    ConstructorEntity *ce = dynamic_cast<ConstructorEntity *>((*classmbr));
    list<Type *>::iterator arg_type(arg_types->begin());
    list<Entity *>::iterator param(ce->formal_params()->begin()),
                             param_end(ce->formal_params()->end());
    for (; param != param_end; ++param, ++arg_type) {
      if ((*param)->kind() != VARIABLE_ENTITY) {
        string errmsg("error: expected variable for formal parameter\n");
        error->type_error(this->lineno(), errmsg, (*param));
        return error_type;
      }

      VariableEntity *ve = dynamic_cast<VariableEntity *>((*param));
      if (!((*arg_type)->isSubtypeOf(ve->type()))) {
        string errmsg("error: actual arguments don't match formal parameters\n");
        error->type_error(this->lineno(), errmsg, (*arg_type), ve->type());
        return error_type;
      }
    }

    // Check for correct modifiers, i.e. that the constructor is PUBLIC.
    if (ce->visibility_flag())
      return new InstanceType(thisclass);

    // If we get here, there is a constructor defined with this name, 
    // but not defined PUBLIC, so access to it is not allowed.
    string errmsg("error: illegal access to constructor\n");
    error->type_error(this->lineno(), errmsg, thisclass);
    return error_type;
  }

  // If we get here, there was no valid constructor defined.
  string errmsg("error: constructor not defined\n");
  error->type_error(this->lineno(), errmsg, thisclass);
  return error_type;
}


// Typeinfer method for ThisExpression and SuperExpression
// If the current method is a static method, then the result type is ClassType 
// if the current method is non-static, then the result type is InstanceType
Type* ThisExpression::typeinfer() 
{
  if (current_scope.method_static_flag)
    return new ClassType(current_scope.class_in_scope);
  return new InstanceType(current_scope.class_in_scope);
}

Type* SuperExpression::typeinfer() 
{
  if (current_scope.method_static_flag)
    return new ClassType(current_scope.class_in_scope);
  return new InstanceType(current_scope.class_in_scope);
}


// Typeinfer method for IdExpression
// If the expression refers to a variable, then the result is 
// that variable’s type. If the expression refers to a class, 
// say x, then the result is ClassType(x).
Type* IdExpression::typeinfer() 
{
  Entity *ident = this->id();
  if (ident->kind() == VARIABLE_ENTITY) {
    VariableEntity *ve = dynamic_cast<VariableEntity *>(ident);
    return ve->type();
  } else if (ident->kind() == CLASS_ENTITY) {
    ClassEntity *ce = dynamic_cast<ClassEntity *>(ident);
    return new ClassType(ce);
  } else {
    string errmsg("error: expected a variable name or a class name\n");
    error->type_error(this->lineno(), errmsg, ident);
    return error_type;
  }
}


// Typeinfer method for NullExpression
Type* NullExpression::typeinfer() { return null_type; }

// Constants
Type* BooleanConstant::typeinfer() { return bool_type; }
Type* IntegerConstant::typeinfer() { return integer_type; }
Type* FloatConstant::typeinfer() { return float_type; }
Type* StringConstant::typeinfer() { return string_type; }
