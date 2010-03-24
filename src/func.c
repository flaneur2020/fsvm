#include "fvm.h"

/*
 * p_test = fnew_proto(op_test);
 * p_main = fnew_proto(op_main);
 *
 * psedo code => 
 *
 *      func foo(a, b) {
 *          var c=100;
 *          return func(d){
 *              puts(a+b+c+d);
 *          }
 *      }
 *
 * compiling =>
 *      op_foo { ...}
 *      op_lambda { ...}
 *
 * Proto *p_lambda = fnew_proto(op_lambda, 1)
 * freg_local_name(p_lambda, 'd');
 * freg_outer_name(p_lambda, 'a');
 * freg_outer_name(p_lambda, 'b');
 * freg_outer_name(p_lambda, 'c');
 *
 * Proto *p_foo = fnew_proto(op_foo, 2);
 * freg_local_name(p_foo, 'a');
 * freg_local_name(p_foo, 'b');
 * freg_local_name(p_foo, 'c');
 * freg_const(p_foo, fnum(100));
 *
 *
 * */

// Proto
Proto* fnew_proto(Op *opcodes, int c_params) {
    Proto *proto = fvm_alloc(Proto);
    proto->opcodes = opcodes;
    proto->c_params = c_params;
    
    return proto;
}

int freg_proto(VM *vm, int pid, Proto *proto){
    int pid = vm->c_protos;
    vm->protos[pid]=proto;
    vm->c_protos++;
    return pid;
}

Proto* fget_proto(VM *vm, int pid){
    if (pid > vm->c_protos || pid < 0){
        fvm_panic("ProtoError: Uncaught Proto: %d", pid);
    } 
    Proto* proto = vm->protos[pid];
    return proto;
}

// fields of Proto
int freg_const(Proto *proto, Obj obj){
    int cid = proto->c_consts;
    proto->consts[cid]=obj;
    proto->c_consts++;
    return cid;
}

Obj fget_const(Proto *proto, int cid){
    if (cid >= proto->c_consts) {
        fvm_panic("ProtoError: consts[%d] do not exists\n", cid);
    }
    return proto->consts[cid];
}

int freg_local_name(Proto *proto, char *str) {
    int lid = proto->c_local_names;
    proto->local_names[lid] = str;
    proto->c_local_names++;
    return lid;
}

int freg_outer_name(Proto *proto, char *str) {
    int lid = proto->c_outer_names;
    proto->outer_names[lid] = str;
    proto->c_local_names++;
    return lid;
}




// Func
Func* fnew_func(Proto *proto, Env* env){
    //check param names
    //the consts in front are all T_STR as names of params
    int i;
    for (i=0;i<proto->c_params;i++){
        if (T(fget_const(proto, i))!=T_STR){
            fvm_panic("ProtoError: consts[%d] should be T_STR as name of param", i);
        }
    }
    Func *func = fvm_alloc(Func);
    Env *new_env = fnew_env(env);
    func->proto = proto;
    func->env = new_env;
    return func;
}



