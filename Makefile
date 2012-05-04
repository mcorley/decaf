# Makefile for decaf compiler

SHELL = /bin/sh

CXX = g++
CXXFLAGS = -g -Wno-deprecated
LEX = flex
LFLAGS = -p -8 -Ce
LIBS = -lfl
YACC = bison
YFLAGS = -t -d -v

OBJECTS = ast.o ast_print.o ast_symbols.o \
					decaf.tab.o driver.o lex.decaf.o \
					symbols.o type_check.o types.o \
					error.o

demo: $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

lex.decaf.o: lex.decaf.c
	$(CXX) $(CXXFLAGS) -o $@ -c $^

lex.decaf.c: decaf.l
	$(LEX) $(LFLAGS) -o $@ $^

decaf.tab.o: decaf.tab.c
	$(CXX) $(CXXFLAGS) -o $@ -c $^

decaf.tab.c: decaf.yy
	$(YACC) $(YFLAGS) -o $@ $^

.PHONY: clean
clean:
	-rm -f demo *.o
	-rm -f lex.decaf.c decaf.tab.c decaf.tab.h decaf.output

