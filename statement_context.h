#ifndef STATEMENT_CONTEXT_H
#define STATEMENT_CONTEXT_H

typedef int Label;

class StatementContext {
public:
  StatementContext();
  StatementContext(StatementContext *old);
  bool loop;
  Label loop_iter;
  Label loop_exit;
};

#endif
