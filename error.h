#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include "ast.h"

using namespace std;

class Error {
public:
  Error(string s);
  void implementation_error(string s);
  void syntax_error(int lineno, string s);
  void type_error(int lineno, string s, Type* t);
  void type_error(int lineno, string s, Type* t1, Type*t2);
  void type_error(int lineno, string s1, char* s2);
  void type_error(int lineno, string s, Entity* e);
  void syntax_error(int lineno, string s1, string s2);

private:
  string sourcefile;
  void prelude(int i);
};

#endif
