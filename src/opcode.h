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
    OP_LOAD_NAME, //_0
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
    OP_FOO, //_0
    //
    // with opr 
    //
    /* stack */
    OP_LOAD_NIL, //_0
    OP_LOAD_NUM, //_4
    OP_LOAD_TMP, //_1
    OP_STORE_TMP,  //_1
    OP_LOAD_CONST,//_1
    /* branch */
    OP_JMP, //_4
    OP_BRANCH, //_4
    /* func */
    OP_MKFUNC, //_1
    OP_CALL, //_1
    // blah~
    OP_LAST
};


#define FVM_OP_NAMES \
    "ret", \
    "nop", \
    "pop", \
    "bind_var", \
    "load_var", \
    "store_var", \
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
    "foo", \
    "LOAD_nil", \
    "LOAD_num", \
    "LOAD_tmp", \
    "pop_tmp", \
    "load_const", \
    "jmp",\
    "branch", \
    "mkfunc", \
    "call", \
    "last"

int fvm_run     ();

#endif
