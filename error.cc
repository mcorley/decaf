#include <iostream>
#include <string>
#include "error.h"

Error::Error(string s) {
  sourcefile = s;
}

void Error::prelude(int lineno) {
  cout << sourcefile << ":" << lineno << ": ";
}

void Error::implementation_error(string s) {
  cout << s << endl;
}

void Error::syntax_error(int lineno, string s) {
  prelude(lineno);
  cout << s << endl;
}

void Error::type_error(int lineno, string s, Type* t) {
  prelude(lineno);
  cout << s << "; found: ";
  t->print();
  cout << endl;
}

void Error::type_error(int lineno, string s, Type* t1, Type* t2) {
  prelude(lineno);
  cout <<  s << "; found ";
  t1->print();
  cout << ", expected ";
  t2->print();
  cout << endl;
}

void Error::type_error(int lineno, string s1, char *s2) {
  prelude(lineno);
  cout << s1 << s2 << endl;
}

void Error::type_error(int lineno, string s, Entity* e) {
  prelude(lineno);
  cout << s ;
  e->print_name();
  cout << endl;
}

void Error::syntax_error(int lineno, string s1, string s2) {
  prelude(lineno);
  cout << s1 << " near token "<< s2 << endl;
}
