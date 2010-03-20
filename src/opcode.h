/*
 * opcode.h
 * Fleurer Lee
 *
 * */

#ifndef opcode_h
#define opcode_h

// opcodes
enum{
    OP_RET = 0, 
    NOP, //_0
    OP_POP, //_0
    OP_BIND_NAME, //_0
    OP_PUSH_NAME, //_0
    OP_STORE_NAME, //_0
    /* arithmetics */
    OP_ADD,  //_0
    OP_SUB, //_0
    /* logic */
    OP_EQ, //_0
    OP_GT, //_0
    OP_LT, //_0
    //
    OP_AND, //_0
    OP_OR,  //_0
    OP_NOT, //_0
    /* blah for debug? */
    OP_PRINT, //_0
    OP_PRINT_STACK, //_0
    //
    OP_PUSH_NIL, //_0
    /////////////////////
    // mark of opr 1
    /////////////////////
    OP_1, 
    OP_MKFUNC, //_1
    OP_CALL, //_1
    /* stack */
    OP_PUSH_TMP, //_1
    OP_STORE_TMP,  //_1
    OP_PUSH_CONST,//_1
    /////////////////////
    // mark of opr 4
    /////////////////////
    OP_4,
    OP_PUSH_NUM, //_4
    OP_JMP, //_4
    OP_BRANCH, //_4
    // blah~
    OP_LAST
};


#define FVM_OP_NAMES \
    "ret", \
    "nop", \
    "pop", \
    "bind_var", \
    "push_var", \
    "pop_var", \
    "add", \
    "sub", \
    "eq", \
    "gt", \
    "lt", \
    "and", \
    "or", \
    "not", \
    "print", \
    "print_stack", \
    "push_nil", \
    "_1", \
    "mkfunc", \
    "call", \
    "push_tmp", \
    "pop_tmp", \
    "push_const", \
    "_4", \
    "push_num", \
    "jmp",\
    "branch", \
    "last"

int fgen     (char **ops, char op_name);
int fgen0    (char **ops, char op_name);
int fgen1    (char **ops, char op_name, char opr);
int fgen4    (char **ops, char op_name, int opr);

char fnext_op    (char **ops);

int fvm_run       ();

#endif
