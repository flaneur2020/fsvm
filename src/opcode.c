#include <stdlib.h>
#include <stdio.h>

#include "opcode.h"
#include "fvm.h"

// main stack {{0_0}}
#define base ((fvm_current())->stack)
#define sp ((fvm_current())->sp)

// just for debug
const char fvm_op_names   [100][100] = { FVM_OP_NAMES };
const char fvm_tag_names  [100][100] = { FVM_TAG_NAMES };

// for opcodes generation



#define _next_op (*(pc++))
#define _next_opr ((int)(*(pc++)))

#define _dbg_show_op (op > OP_1)?\
    printf("op:%s,%d\n", fvm_op_names[(int)op], *(pc)): \
    printf("op:%s\n", fvm_op_names[(int)op]);

Obj fvm_run(Proto *proto, Env *env) {
    if (FDEBUG) {_dbg_show_proto(proto);};

    Op op, *pc;
    pc = proto->opcodes;
    while ((op = _next_op) != OP_RET ) {
        _dbg_show_op;
        switch (op){
        case NOP: break;
                                /* stack */
        case OP_LOAD_NIL: {
                                fpush(fnil());
        } break;     

        case OP_LOAD_NUM: {
                                int n = _next_opr;
                                Obj r = fnum(n);
                                fpush(r);
        } break;

        case OP_LOAD_CONST: {
                                int n = _next_opr;            
                                Obj r = fget_const(proto, n);
                                fpush(r);
        } break;

        case OP_LOAD_LOCAL: {
                                int n = _next_opr;
                                Obj r = fget_local(env, n);
                                fpush(r);
        } break;

        case OP_STORE_LOCAL: {
                                int n = _next_opr;
                                fset_local(env, n, fpop());
        } break;

        case OP_LOAD_OUTER: {
                                int n = _next_opr;                  
                                Obj r = fget_outer(env, n);
                                fpush(r);
        } break;

        case OP_STORE_OUTER: {
                                int n = _next_opr;             
                                fset_outer(env, n, fpop());
        } break;

        case OP_LOAD_NAME: {
                                char *name = Vstr(fpop());
                                Obj r = fget_name(env, name);
                                if (fis_nil(r)){
                                    fvm_panic("NameError: <%s> is not availible\n", name);
                                }
                                fpush(r);
        } break;

        case OP_STORE_NAME: {
                                char *name = Vstr(fpop());
                                Obj v = fpop();
                                Obj r = fset_name(env, name, v);
                                if (fis_nil(r)){
                                    fvm_panic("NameError: <%s> is not availible\n", name);
                                }
        } break;
        
        case OP_POP: {
                                fpop();            
        } break;
        
                                /* arithmetics */
        case OP_ADD: {
                                Obj a, b, r;
                                a = fpop();
                                b = fpop();
                                r = fnum( Vnum(a) + Vnum(b) );
                                fpush(r);
        } break;

        case OP_SUB: {
                                Obj a, b, r;
                                a = fpop();
                                b = fpop();
                                r = fnum( Vnum(a) - Vnum(b) );
                                fpush(r);      
        } break;

        case OP_MUL: {
                                Obj a, b, r;     
                                a = fpop();
                                b = fpop();
                                r = fnum( Vnum(a) * Vnum(b) );
                                fpush(r);
        } break;

        case OP_DIV: {
                                Obj a, b, r;     
                                a = fpop();
                                b = fpop();
                                r = fnum( Vnum(a) / Vnum(b) );
                                fpush(r);
        } break;

        case OP_MOD: {
                                Obj a, b, r;     
                                a = fpop();
                                b = fpop();
                                r = fnum( Vnum(a) % Vnum(b) );
                                fpush(r);
        } break;

                                /* logic */
        case OP_EQ: {
                                Obj a, b;    
                                a = fpop();
                                b = fpop();
                                if ( feq( a, b ) ){
                                    fpush(b);
                                }
                                else {
                                    fpush(fnil());
                                }
        } break;

        case OP_GT: {
                                Obj a, b;    
                                a = fpop();
                                b = fpop();
                                if ( fgt( a, b ) ){
                                    fpush(b);
                                }
                                else {
                                    fpush(fnil());
                                }  
        } break;

        case OP_LT: {
                                Obj a, b;    
                                a = fpop();
                                b = fpop();
                                if ( flt( a, b ) ){
                                    fpush(b);
                                }
                                else {
                                    fpush(fnil());
                                }  
        } break;

        case OP_AND: {
                                Obj a, b;
                                a = fpop();
                                b = fpop();
                                if (fnot_nil(a) && fnot_nil(b)) {
                                    fpush(b);
                                } else {
                                    fpush(fnil());
                                }
        } break; 

        case OP_OR: {
                                Obj a, b;     
                                a = fpop();
                                b = fpop();
                                if (fnot_nil(a)) {
                                    fpush(a);
                                }
                                else if (fnot_nil(b)) {
                                    fpush(b);
                                }
                                else {
                                    fpush(fnil()); 
                                }
        } break;

        case OP_NOT:{
                                Obj a, r;    
                                a = fpop();
                                if (fis_nil(a)) {
                                    r = fnum(1);
                                }
                                else {
                                    r = fnil();
                                }
                                fpush(r);
        } break;

        case OP_JMP: {
                                int offset = _next_opr;     
                                if (offset < 0){
                                    offset -= 2;
                                }
                                pc += offset;
        } break;
        
        case OP_BRANCH: {
                                Obj a = fpop();
                                int offset = _next_opr;     
                                if (fnot_nil(a)){
                                    // same as jmp
                                    if (offset < 0){
                                        offset -= 2;
                                    }
                                    pc += offset;
                                }
        } break;
                                /* operations */
        case OP_MKFUNC: {
                                int pid = _next_opr;
                                Proto *proto = env->vm->protos[pid];
                                OFunc *func = fnew_func(proto, env);
                                Obj   ofunc = (Obj) func;
                                fpush(ofunc);
        } break;

        case OP_CALL: {
                                int n = _next_opr;
                                Obj obj = *sp--;
                                fcall(obj, n);
        } break;

        case OP_PRINT: {
                                Obj a = fpop();
                                char *str = Vstr(a);
                                printf("%s\n", str);
        } break;

        case OP_PRINT_STACK: {
                                 Obj* stack = base;
                                 int i=0, offset=sp-stack+1;
                                 printf("stack depth: %d\n", offset);
                                 for (i=offset-1; i>=0; i--){
                                     printf("%3d | %8s | %s\n", i, fvm_tag_names[T(stack[i])], fto_cstr(stack[i]));
                                 }             
        } break;

        default:
                                fvm_panic("VMError: unknown opcode: %d\n", *pc);
        }
    }
    return *sp;
}


// stack helpers 
// TODO: addes overflow check~
Obj fpush(Obj obj) {
    *(++sp) = obj;
    return obj;
}

Obj fpop() {
    if (sp-1 < base) {
        fvm_panic("Stack Error: pop below the stack base\n");
    }
    return *(sp--);
}

