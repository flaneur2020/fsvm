#include "fvm.h"

/*
 * p_test = fnew_proto(op_test);
 * p_main = fnew_proto(op_main);
 *
 * f_test = fnew_func(p_test, env);
 * f_main = fnew_func(p_main, env);
 * 
 * *******************************************
 * Proto:
 *      consts =
 *           0 - t_str:'a'
 *           1 - t_str:'b'
 *      params_c = 2
 * Env:
 *      'a': 1
 *      'b': 2
 * -------------------------
 * Op op_add[] = {
 *      OP_PUSH_CONST, 0,
 *      OP_PUSH_VAR,
 *      OP_PUSH_CONST, 1,
 *      OP_PUSH_VAR,
 *      OP_ADD,
 *      OP_RET
 * };
 * char pnames[10][255]={
 *      "a", "b"
 * };
 * p_add = fnew_proto(op_add, 2, pnames);
 *
 * */

Proto* fnew_proto(Op *opcodes, int c_params) {
    Proto *proto = fvm_alloc(Proto);
    proto->opcodes = opcodes;
    proto->c_params = c_params;
    
    return proto;
}

int fset_const(Proto *proto, int cid, Obj obj){
    proto->c_consts++;
    proto->consts[cid]=obj;
    return cid;
}

Obj fget_const(Proto *proto, int cid){
    if (cid >= proto->c_consts) {
        fvm_panic("ProtoError: consts[%d] do not exists\n", cid);
    }
    return proto->consts[cid];
}

int fset_proto(Env *env, int pid, Proto *proto){
    env->vm->c_protos++;
    env->vm->protos[pid]=proto;
    return pid;
}

Proto* fget_proto(Env *env, int pid){
    if (pid > env->vm->c_protos || pid < 0){
        fvm_panic("ProtoError: Uncaught Prototype: %d", pid);
    } 
    Proto* proto = env->vm->protos[pid];
    return proto;
}

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



