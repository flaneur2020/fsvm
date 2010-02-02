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
    NOP,
    OP_POP,
    OP_BIND_VAR,
    OP_PUSH_VAR,
    OP_POP_VAR,
    /* arithmetics */
    OP_ADD,
    OP_SUB,
    /* logic */
    OP_EQ,
    OP_GT,
    OP_LT,
    //
    OP_AND,
    OP_OR,
    OP_NOT,
    /* blah for debug? */
    OP_PRINT,
    OP_PRINT_STACK,
    OP_FOO, 
    //
    // with opr 
    //
    /* stack */
    OP_PUSH_NIL, //opr
    OP_PUSH_NUM, //opr
    OP_PUSH_TMP, //opr
    OP_POP_TMP,  //opr
    OP_PUSH_CONST,//opr
    /* branch */
    OP_JMP, //opr
    OP_BRANCH, //opr
    /* func */
    OP_MKFUNC, //opr
    OP_CALL, //opr
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
    "foo", \
    "push_nil", \
    "push_num", \
    "push_tmp", \
    "pop_tmp", \
    "push_const", \
    "jmp",\
    "branch", \
    "mkfunc", \
    "call", \
    "last"

int fvm_run     ();

#endif
