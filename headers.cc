#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include "symbols.h"
#include "ast_symbols.h"
#include "ast.h"
#include "error.h"

#define NSTRINGS  100
//Initial size for string table.
#define MAXSTRLEN 100

#define NIMPORTS 128

int* string_table;
int string_table_extent;
int string_table_offset;
int* string_index;
int current_string_index;
int string_index_extent;

int import_index;
char* import_table[NIMPORTS];

extern Error* error;
extern ofstream codefile;

void initialize_table_data() {
  string_table = (int *)malloc(NSTRINGS*MAXSTRLEN);
  string_index = (int *)malloc(NSTRINGS*sizeof(int));
  string_table_extent = NSTRINGS*MAXSTRLEN;
  string_index_extent = NSTRINGS;
  string_table_offset = 0;
  current_string_index = 0;
  import_index = 0;
}

int install_string(char *s) {
  if (string_table_offset + strlen(s)+1 >= string_table_extent-1) {
    string_table_extent += NSTRINGS*(strlen(s) + 1);
    string_table = (int *)realloc(string_table, string_table_extent);
  }
  strcpy((char *) (&(string_table[string_table_offset])), s);
  int retval = string_table_offset;

  if (current_string_index >= string_index_extent-1) {
    string_index_extent += NSTRINGS;
    string_index = (int *)realloc(string_index, string_index_extent*sizeof(int));
  }
  string_table_offset += 1 + (strlen(s) + 1)/sizeof(int);

  if (current_string_index >= string_index_extent-1) {
    fprintf(stderr, "String table full");
    exit(1);
  }
  string_index[current_string_index++] = retval;
  return retval;
  // assuming all mallocs succeed.  Will come fix this later...
}

void generate_headers(Program *p) {
  int i;
  // generics
  codefile <<  "#include <stdio.h> \n\
#include <string.h>\n\
#include \"machine.h\"\n\
#include \"inst.h\"" << endl;
    // imports

    for(i=0; i<import_index; i++) {
      codefile << "#include \"" << import_table[i] << ".c\"" << endl;
    }
  codefile << "entry static_area_initialization() {" << endl;
  // string table
  for(i=0; i<current_string_index; i++) {
    codefile << "strcpy(";
    codefile << "(char *) (&(static_area[" << string_index[i] << "])), " ;
    codefile << "\"" << (char *)(& (string_table[string_index[i]])) << "\");" << endl;
  }
  // region table
  list<ClassEntity*> *cl = p->classes();
  list<ClassEntity*>::iterator ci;
  int class_offset = string_table_offset;
  codefile << "region_table[0] = 0;" << endl;
  for(ci=cl->begin(); ci != cl->end(); ci++) {
    ClassEntity *ce = *ci;
    codefile << "region_table[" << (ce->class_number()) << "] = ";
    codefile << class_offset << ";" << endl;
    class_offset += ce->class_size();
  }
  codefile << "}" << endl << endl;
}

void generate_main(Program *p) {
  // find the last "static void main()" in the program.
  MethodEntity *main_method = NULL;
  list<ClassEntity*> *cl = p->classes();
  list<ClassEntity*>::iterator ci;
  for(ci=cl->begin(); ci != cl->end(); ci++) {
    ClassEntity *ce = *ci;
    list<Entity*> *ml = ce->class_members();
    list<Entity*>::iterator mi;
    for(mi=ml->begin(); mi != ml->end(); mi++) {
      if ((*mi)->kind() == METHOD_ENTITY) {
        MethodEntity *m = (MethodEntity*)(*mi);
        if ((strcmp(m->name(), "main") == 0) &&
            (m->formal_params()->size() == 0) &&
            (m->static_flag()) &&
            (m->return_type()->isSubtypeOf(new VoidType())))
          main_method = m;
      }
    }
  }
  if (main_method == NULL) {
    error->implementation_error("Program has no main method");
    exit(0);
  }
  codefile << "entry sugar_main() { " << endl;
  codefile << "new_env(0,0);" << endl;
  codefile << "call(" << main_method->distinct_name() << ");" << endl;
  codefile << "ret();" << endl << "}" << endl;
}


