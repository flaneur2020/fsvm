#ifndef opcode_h
#define opcode_h

// opcodes
enum{
    OP_RET = 0, 
    NOP, 
    OP_DUP,
    OP_POP, 
    OP_BIND_NAME, 
    OP_LOAD_NAME, 
    OP_STORE_NAME, 
    /* arithmetics */
    OP_ADD,  
    OP_SUB, 
    OP_MUL, 
    OP_DIV, 
    OP_MOD, 
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
    OP_LOAD_NIL, 
    OP_1, 
    //
    // with opr 
    //
    /* stack */
    OP_LOAD_NUM,        
    OP_LOAD_CONST,      
    OP_LOAD_LOCAL,      
    OP_STORE_LOCAL,     
    OP_LOAD_OUTER,      
    OP_STORE_OUTER,     
    //
    OP_MKFUNC,          
    OP_CALL,            
    /* branch */
    OP_JMP,             
    OP_BRANCH,          
    // blah~
    OP_LAST
};


#define FVM_OP_NAMES \
    "ret", \
    "dup", \
    "nop", \
    "pop", \
    "bind_var", \
    "load_var", \
    "store_var", \
    "add", \
    "sub", \
    "mul", \
    "div", \
    "mod", \
    "eq", \
    "gt", \
    "lt", \
    "and", \
    "or", \
    "not", \
    "print", \
    "print_stack", \
    "_1", \
    "load_nil", \
    "load_num", \
    "load_const", \
    "load_local", \
    "store_local", \
    "load_outer", \
    "store_outer", \
    "mk_func", \
    "call", \
    "jmp",\
    "branch", \
    "last"


#endif
