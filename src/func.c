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

int freg_proto(VM *vm, Proto *proto){
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

// local_name
int freg_lname(Proto *proto, char *str) {
    int id = proto->c_locals;
    proto->lnames[id] = str;
    proto->c_locals++;
    return id;
}

//outer_name
int freg_oname(Proto *proto, char *str) {
    int id = proto->c_outers;
    proto->onames[id] = str;
    proto->c_outers++;
    return id;
}




// Func
Func* fnew_func(Proto *proto, Env* parent){
    //check param names
    //the consts in front are all T_STR as names of params
    // TODO: parameters are the top locals
    // init the locals
    Func *func = fvm_alloc(Func);
    func->proto = proto;
    func->env = parent;

    // TODO: init all the names from proto
    return func;
}

Obj fcall(Func* func, int argc) {
    Proto   *proto = func->proto;
    if (argc < proto->c_params) {
        fvm_panic("ApplyError: Func:<0x%lx> params do not match. (%d of %d)", (Addr)func, argc, proto->c_params);
    }

    // make a closure
    int c_locals = proto->c_locals;
    int c_outers = proto->c_outers;
    Env *env = fnew_env(func->env, c_locals, c_outers);
    // init all the locals as nil
    int i;
    for(i=0; i<proto->c_locals; i++){
        char *name = proto->lnames[i];
        env->locals[i].name = name;
        Obj  *ref  = fset_local(env, i, fnil());
        fbind_name(env, name, ref);
    }
    // init all the outers as NULL
    for(i=0; i<proto->c_outers; i++){
        char *name = proto->onames[i];
        env->outers[i].name = name;
        env->outers[i].ref  = NULL;
    }

    // pop params
    for(i=0;i<argc;i++){
        Obj o = fpop();
        fset_local(env, i, o);
    }
    
    // TODO: do some clean here
    // env can be cleaned now . TODO: if refcount were introduced, map (-1) vars.ref
    return fvm_run(proto, env);
}


