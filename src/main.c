#include <stdio.h>
#include <stdlib.h>

#include "opcode.h"
#include "fvm.h"

/* 
 * for debug & test
 * some helpers
 * test  
 * */
/*
void test_ret(){
    Op op_main[256] = {
        OP_MKFUNC, 1,
        OP_PRINT_STACK,
        OP_CALL, 0,
        OP_RET,
        OP_LOAD_NUM, 2,
        //OP_JMP, -3,
        OP_LOAD_NUM, 1,
        OP_LT,
        OP_BRANCH, 1,
        OP_PRINT_STACK,
        OP_RET,
        OP_LOAD_NUM, 1,
        OP_LOAD_NUM, 1,
        OP_ADD,
        OP_PRINT,
        OP_LOAD_TMP, 0,
        OP_PRINT_STACK,
        OP_CALL, 0,
        OP_LOAD_NUM, 1,
        OP_ADD,
        OP_RET,
    };
    Op op_test[256] = {
        OP_LOAD_NUM, 16,
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
        OP_LOAD_CONST, 0,
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
        OP_LOAD_NUM, 16,
        OP_LOAD_NUM, 17,
        OP_LOAD_CONST, 0,
        OP_LOAD_NAME,
        OP_PRINT_STACK,
        OP_CALL, 2,
        OP_PRINT_STACK,
        OP_RET
    };
    Op op_blah[256]={
        OP_LOAD_CONST, 0,
        OP_LOAD_NAME,
        OP_LOAD_CONST, 1,
        OP_LOAD_NAME,
        OP_PRINT_STACK,
        OP_ADD,
        OP_LOAD_NUM, 1,
        OP_ADD,
        OP_RET
    };

    Proto *p_main = fnew_proto(op_main, 0, NULL);
    char *pnames[10];
    pnames[0]="a";
    pnames[1]="b";

    Proto *p_blah = fnew_proto(op_blah, 2, pnames);

    Env *env=fnew_env(NULL);
    OFunc* f_main=fnew_func(p_main, env);
    OFunc* f_blah=fnew_func(p_blah, env);

    Obj o_main=ffunc(f_main);
    Obj o_blah=ffunc(f_blah);
    
    fbind_var((fvm_current())->root, "blah", o_blah);
    fset_const(f_main->proto, 0, fstr("blah"));

    fio_puts(fcall(0, f_main));
}

int test_rec(){
    Op op_main[]={
        OP_LOAD_NUM, 10,
        OP_LOAD_CONST, 0,  
        OP_LOAD_NAME,
        OP_CALL, 1,
        OP_RET
    };
    Op op_sum[]={
        OP_LOAD_CONST, 0, //"i"
        OP_LOAD_NAME, 
        OP_STORE_TMP, 0, //store i
        OP_LOAD_TMP, 0, //push i
        OP_LOAD_NUM, 0, 
        OP_EQ,
        OP_NOT, // i!=0?
        OP_BRANCH, 3, 
            OP_LOAD_NUM, 0,
            OP_RET,
        //else
            OP_LOAD_TMP, 0, 
            OP_LOAD_NUM, 1, //1
            OP_SUB, 
            OP_LOAD_CONST, 1, //"sum"
            OP_LOAD_NAME, 
            OP_CALL, 1,   //sum(tmp[0]-1)
            OP_LOAD_TMP, 0, 
            OP_PRINT_STACK, 
            OP_ADD, //tmp[0]+sum(tmp[0]-1)
        OP_RET
    };
    Env *env=fnew_env(NULL);
    
    Proto *p_main = fnew_proto(op_main, 0);
    fset_const(p_main, 0, fstr("sum"));
    OFunc* f_main=fnew_func(p_main, env);
    Obj o_main=ffunc(f_main);

    Proto* p_sum=fnew_proto(op_sum, 1);
    fset_const(p_sum, 0, fstr("i"));
    fset_const(p_sum, 1, fstr("sum"));
    OFunc* f_sum=fnew_func(p_sum, env);
    Obj o_sum=ffunc(f_sum);

    fbind_var(env, "sum", o_sum);
    
    fio_puts(fcall(0, f_main));
    return 0;
}
*/

int test_add(){
    Env *env=NULL;

    Op c_add[]={
        OP_LOAD_LOCAL, 0,
        OP_PRINT_STACK,
        OP_LOAD_LOCAL, 1,
        OP_ADD,
        OP_RET
    };
    
    Proto *p_add=fnew_proto(c_add, 0);
    freg_lname(p_add, "a");
    freg_lname(p_add, "b");
    OFunc *f_add = fnew_func(p_add, env);

    Op c_main[]={
        OP_LOAD_CONST, 0,
        OP_LOAD_CONST, 1,
        OP_LOAD_CONST, 2,
        OP_PRINT_STACK,
        OP_CALL, 2,
        OP_RET
    };
    Proto *p_main=fnew_proto(c_main, 0);
    freg_const(p_main, fnum(1));
    freg_const(p_main, fnum(6));
    freg_const(p_main, f_add);
    OFunc *f_main = fnew_func(p_main, env);

    Obj o = fcall(f_main, 0);
    fio_puts(o); //expected 7
    return 0;
}

int test_outer() {

    Op c_inc[] = {
        OP_LOAD_OUTER, 0, 
        OP_LOAD_NUM, 1,
        OP_ADD,
        OP_STORE_OUTER, 0,
        OP_LOAD_OUTER, 0,
        OP_RET
    };
    Proto *p_inc = fnew_proto(c_inc, 0);
    freg_oname(p_inc, "sum");
    int pid_inc = freg_proto(p_inc);

    Op c_mk_counter[] = {
        OP_LOAD_NUM, 1,
        OP_STORE_LOCAL, 0,
        OP_MKFUNC, pid_inc,
        OP_RET
    };
    Proto *p_mk_counter = fnew_proto(c_mk_counter, 0);
    freg_lname(p_mk_counter, "sum");
    OFunc *o_mk_counter = fnew_func(p_mk_counter, NULL);

    Op c_main[] = {
        OP_LOAD_CONST, 0,
        OP_CALL, 0,
        OP_STORE_LOCAL, 0, 
        OP_LOAD_LOCAL, 0, 
        OP_CALL, 0,
        OP_LOAD_LOCAL, 0, 
        OP_CALL, 0,
        OP_LOAD_LOCAL, 0, 
        OP_CALL, 0,
        OP_RET
    };
    Proto *p_main = fnew_proto(c_main, 0);
    freg_const(p_main, o_mk_counter);
    freg_lname(p_main, "f");
    OFunc *o_main = fnew_func(p_main, NULL);
    
    Obj r = fcall(o_main, 0);
    fio_puts(r);
    puts("~~~~~~~~~~~~~~~~~~");
    return 0;
}

int test_num(){
    Op op_main[]={
        OP_LOAD_NUM, 9,
        OP_LOAD_NUM, 1,
        OP_PRINT_STACK,
        OP_SUB, 
        OP_PRINT_STACK,
        OP_RET
    };
    Proto *p_main = fnew_proto(op_main, 0);
    OFunc *o_main = fnew_func(p_main, 0);

    Obj r = fcall(o_main, 0);
    fio_puts(r);
    return 0;
}

int test_type() {
    Op c_main[]={
        OP_LOAD_CONST, 0,
        OP_LOAD_CONST, 1,
        OP_ADD,
        OP_LOAD_CONST, 2,
        OP_PRINT_STACK,
        OP_RET
    };
    Proto *p_main=fnew_proto(c_main, 0);
    freg_const(p_main, fnum(3));
    freg_const(p_main, fnum(1));
    freg_const(p_main, fstr("str"));
    freg_const(p_main, fstr("abc"));

    OFunc *f_main = fnew_func(p_main, NULL);

    Obj o = fcall(f_main, 0);
    fio_puts(o);
    return 0;
}

int test_cfunc(){
    Obj inc(int argc, Obj *argv){
        Obj a = argv[0];
        return fnum(fto_cint(a)+1);
    }
    CFunc *f_inc = fnew_cfunc(inc, 1);

    Op op_main[] = {
        OP_LOAD_NUM, 7,
        OP_PRINT_STACK,
        OP_LOAD_CONST, 0,
        OP_CALL, 1,
        OP_PRINT_STACK,
        OP_RET
    };
    Proto *p_main=fnew_proto(op_main, 0);
    freg_const(p_main, f_inc);

    OFunc *f_main = fnew_func(p_main, NULL);
    Obj o = fcall(f_main, 0);
    return 0;
}

int test_42(){
    Op op_main[]={
        OP_LOAD_NUM, 42,
        OP_PRINT_STACK,
        OP_PRINT,
        OP_RET
    };
    Proto *p_main = fnew_proto(op_main, 0);
    OFunc *o_main = fnew_func(p_main, 0);

    Obj r = fcall(o_main, 0);
    return 0;
}

int test_dup(){
    Op op_main[]={
        OP_LOAD_NUM, 42,
        OP_DUP,
        OP_PRINT_STACK,
        OP_RET
    };
    Proto *p_main = fnew_proto(op_main, 0);
    OFunc *o_main = fnew_func(p_main, 0);

    Obj r = fcall(o_main, 0);
    return 0;
}

int main(int argc, const char *argv[])
{
    fvm_init();
    //test_rec();
    //test_add();
    //test_outer();
    //test_num();
    //test_cfunc();
    test_dup();
    //test_type();
    return 0;
}




