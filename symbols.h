
#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <cstring>
#include <list>
#include <ext/hash_map>

using namespace std;
using namespace __gnu_cxx;

// Kind: enumerated data type indicating the kind of a name
enum Kind {
  CLASS_ENTITY, 
  METHOD_ENTITY, 
  CONSTRUCTOR_ENTITY, 
  FIELD_ENTITY, 
  VARIABLE_ENTITY
};

class Entity {
  public:
    Entity();
    Entity(const char* name, Kind kind);
    virtual ~Entity();

    // read
    virtual const char* name() const;
    virtual Entity* same_name() const;
    virtual Entity* same_scope() const;
    virtual int level_number() const;
    virtual Kind kind() const;

    // write
    virtual void same_name(Entity* );
    virtual void same_scope(Entity* );
    virtual void level_number(int);

    // print
    virtual void print();
    virtual void print_name();

  private:
    const Entity& operator=(const Entity&);
    // Disable assignment, or if you want to permit it, define it explicitly.
    // Avoid using the default assignment operation which will copy field-wise.

  private:
    const char* name_;
    Kind kind_;
    Entity *same_name_;
    Entity *same_scope_;

    int level_number_;
    int entity_number_;
};

struct eqstr {
  bool operator()(const char* s1, const char* s2) const {
    return strcmp(s1, s2) == 0;
  }
};

class EntityTable {
  public:
    EntityTable();
    virtual ~EntityTable();

    //API:
    virtual Entity* find_entity(const char *name, Kind kind, bool *current);
    virtual void add_entity(Entity* entity);

    virtual void enter_block();
    virtual void leave_block();

    virtual list<Entity*> defined_in_current_scope();
    virtual void print_symbols_in_scope();

  private:
    const Entity& operator=(const Entity&);
    // Disable assignment, or if you want to permit it, define it explicitly.
    // Avoid using the default assignment operation which will copy field-wise.

  private:
    int level_;
    list<Entity*> scope_stack_;  
    hash_map<const char *, Entity*, hash<const char *>, eqstr > name_table_;
    void print_name_table_entries();
};

#endif // SYMBOLS_H
