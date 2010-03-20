#include <stdio.h>
#include <stdlib.h>

#include "opcode.h"
#include "fvm.h"

/* 
 * for debug & test
 * some helpers
 * test  
 * */

void test_ret(){
    Op op_main[256] = {
        OP_MKFUNC, 1,
        OP_PRINT_STACK,
        OP_CALL, 0,
        OP_RET,
        OP_PUSH_NUM, 2,
        //OP_JMP, -3,
        OP_PUSH_NUM, 1,
        OP_LT,
        OP_BRANCH, 1,
        OP_PRINT_STACK,
        OP_RET,
        OP_PUSH_NUM, 1,
        OP_PUSH_NUM, 1,
        OP_ADD,
        OP_PRINT,
        OP_PUSH_TMP, 0,
        OP_PRINT_STACK,
        OP_CALL, 0,
        OP_PUSH_NUM, 1,
        OP_ADD,
        OP_RET,
    };
    Op op_test[256] = {
        OP_PUSH_NUM, 16,
        OP_PRINT_STACK,
        OP_RET
    };
    
    Obj f_main, f_test;
    Proto *p_main, *p_test;
    Env *env;

    env = fnew_env();
    p_main = fnew_proto(op_main, 0, NULL);
    p_test = fnew_proto(op_test, 0, NULL);

    f_main = ffunc(fnew_func(p_main, env));
    f_test = ffunc(fnew_func(p_test, env));

    env->vm->protos[1]=p_test;

    fset_tmp(Vfunc(f_main)->env, 0, f_test);

    Obj r;
    r = fcall(0, f_main);
    //TODO: move stack into VM
    //printf("%d\n", r.val); // 18 expected
}

int test_var(){
    Obj a=fnum(16);
    Env *env=fnew_env(NULL);
    fio_puts(fget_var(env, "a"));

    fbind_var(env->parent, "a", a);
    fio_puts(fget_var(env, "a"));
    fset_var(env, "a", fnum(12));
    fio_puts(fget_var(env->parent, "a"));
    
    fbind_var(env, "a", fnum(123));
    fio_puts(fget_var(env, "a"));
    fio_puts(fget_var(env->parent, "a"));
    return 0;
}

int test_const(){
    Op op_main[256] = {
        OP_PUSH_CONST, 0,
        OP_PRINT_STACK,
        OP_RET
    };
    printf("---------%d\n", feq(fstr("test"), fstr("test")));
    Proto *p_main=fnew_proto(op_main, 0, NULL);
    fset_const(p_main, 0, fstr("test"));
    Obj f_main=ffunc(fnew_func(p_main, NULL));
    fcall(0, f_main);
    return 0;
}

int test_funcall(){
    Op op_main[256]={
        OP_PUSH_NUM, 16,
        OP_PUSH_NUM, 17,
        OP_PUSH_CONST, 0,
        OP_PUSH_NAME,
        OP_PRINT_STACK,
        OP_CALL, 2,
        OP_PRINT_STACK,
        OP_RET
    };
    Op op_blah[256]={
        OP_PUSH_CONST, 0,
        OP_PUSH_NAME,
        OP_PUSH_CONST, 1,
        OP_PUSH_NAME,
        OP_PRINT_STACK,
        OP_ADD,
        OP_PUSH_NUM, 1,
        OP_ADD,
        OP_RET
    };

    Proto *p_main = fnew_proto(op_main, 0, NULL);
    char *pnames[10];
    pnames[0]="a";
    pnames[1]="b";

    Proto *p_blah = fnew_proto(op_blah, 2, pnames);

    Env *env=fnew_env(NULL);
    Func* f_main=fnew_func(p_main, env);
    Func* f_blah=fnew_func(p_blah, env);

    Obj o_main=ffunc(f_main);
    Obj o_blah=ffunc(f_blah);
    
    fbind_var((fvm_current())->root, "blah", o_blah);
    fset_const(f_main->proto, 0, fstr("blah"));

    fio_puts(fcall(0, f_main));
}

int test_rec(){
    Op op_main[]={
        OP_PUSH_NUM, 10,
        OP_PUSH_CONST, 0,  
        OP_PUSH_NAME,
        OP_CALL, 1,
        OP_RET
    };
    Op op_sum[]={
        OP_PUSH_CONST, 0, //"i"
        OP_PUSH_NAME, 
        OP_STORE_TMP, 0, //store i
        OP_PUSH_TMP, 0, //push i
        OP_PUSH_NUM, 0, 
        OP_EQ,
        OP_NOT, // i!=0?
        OP_BRANCH, 3, 
            OP_PUSH_NUM, 0,
            OP_RET,
        //else
            OP_PUSH_TMP, 0, 
            OP_PUSH_NUM, 1, //1
            OP_SUB, 
            OP_PUSH_CONST, 1, //"sum"
            OP_PUSH_NAME, 
            OP_CALL, 1,   //sum(tmp[0]-1)
            OP_PUSH_TMP, 0, 
            OP_PRINT_STACK, 
            OP_ADD, //tmp[0]+sum(tmp[0]-1)
        OP_RET
    };
    Env *env=fnew_env(NULL);
    
    Proto *p_main = fnew_proto(op_main, 0);
    fset_const(p_main, 0, fstr("sum"));
    Func* f_main=fnew_func(p_main, env);
    Obj o_main=ffunc(f_main);

    Proto* p_sum=fnew_proto(op_sum, 1);
    fset_const(p_sum, 0, fstr("i"));
    fset_const(p_sum, 1, fstr("sum"));
    Func* f_sum=fnew_func(p_sum, env);
    Obj o_sum=ffunc(f_sum);

    fbind_var(env, "sum", o_sum);
    
    fio_puts(fcall(0, f_main));
    return 0;
}

int test_gen() {
    char codes[128];
    char *ops=codes;
    fgen(&ops, 0);
    fgen(&ops, 1);
    fgen(&ops, 2);
    int i=0;
    for(;i<3;i++) {
        printf("%d\n", codes[i]);
    }

    ops=codes;
    fgen0(&ops, OP_RET);
    fgen1(&ops, OP_PUSH_TMP, 2);
    fgen1(&ops, OP_PUSH_TMP, 2);
    fgen4(&ops, OP_PUSH_NUM, 1000);
    fgen4(&ops, OP_PUSH_NUM, 1000);
    fgen1(&ops, OP_PUSH_TMP, 2);
    fgen4(&ops, OP_PUSH_NUM, 1000);

    char *op=codes;
    for(i=0;i<7;i++){
        fprint_op(op);
        op=fnext_op(op);
    }
    return 0;
}

int main(int argc, const char *argv[])
{
    fvm_init();
    //test_ret();
    //test_const();
    //test_var();
    //test_funcall();
    test_rec();
    test_gen();
    return 0;
}




