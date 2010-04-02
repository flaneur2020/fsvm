#include <stdio.h>
#include <string.h>

#include "fvm.h"


// vm is global
static VM *vm=NULL;
VM* fvm_init() {
    static VM     _vm;
    static Proto  *protos[255];
    static Obj    stack[255];
    if (vm==NULL){
        GC_INIT();
        _vm.stack=stack;
        _vm.sp=stack;
        _vm.protos=protos;
        _vm.c_protos=0;
        //init the first env as root
        _vm.root=fvm_alloc(Env);
        _vm.root->parent=NULL;
        _vm.root->h_locals=kh_init(str);
        _vm.root->vm=&_vm;
        vm=&_vm;
    }
    return vm;
}

VM* fvm_current() {
    return vm;
}

// Env do NOT need parent!
Env* fnew_env(size_t c_locals, size_t c_outers, Var* ovars) {
    Env* env = fvm_alloc(Env);
    env->vm = fvm_current();
    env->h_locals = kh_init(str);
    //it have 16 locals at least
    c_locals = c_locals > 16 ? c_locals : 16;
    env->c_locals = c_locals;
    env->c_outers = c_outers;
    // allocate all the Vars and Objs, 
    // TODO: memset as 0!
    env->locals = fvm_malloc( c_locals * sizeof(Var) );
    int i;
    for(i=0; i<c_locals; i++) {
        env->locals[i].name = NULL;
        env->locals[i].ref  = fvm_alloc(Obj);
    }

    // init all the ovars in the hash
    env->outers = ovars;
    if (ovars != NULL){
        for(i=0; i<c_outers; i++){
            freg_binding(env, &ovars[i]);
        }
    }
    return env;
}

//TODO: you know
int fdel_env(Env *env){
    //fvm_free(env->h_locals);
    //int i;
    //for(i=0; i<env->c_locals; i++){
        //
    //}
    //fvm_free(env->locals);
    //fvm_free(env->outers);
    // TODO: make each env->children->parent = env->parent;
    //fvm_free(env);
}

Obj fget_local(Env *env, int id) {
    Var *v = &env->locals[id];
    return *(v->ref);
}

Obj* fset_local(Env *env, int id, Obj obj) {
    //TODO: add boundary check 
    Var *v = &(env->locals[id]);
    *(v->ref) = obj;
    return v->ref;
}

Obj fget_outer(Env *env, int id) {
    Var *v = &(env->outers[id]);
    return *(v->ref);
}

Obj fset_outer(Env *env, int id, Obj obj){
    Var *v = &(env->outers[id]);
    *(v->ref) = obj;
    return *(v->ref);
}

Var* freg_binding(Env *env, Var *var) {
    khiter_t k; int ret;
    khash_t(str) *h=env->h_locals;
    k=kh_put(str, h, var->name, &ret);
    if (!ret){
        kh_del(str, h, k);
    }
    kh_val(h, k) = var;
    return var;
}

Var* fget_binding(Env *env, char *name) {
    while (env != NULL){
        khash_t(str) *h; khiter_t k; int missing;
        h=env->h_locals;
        k=kh_get(str, h, name);
        missing = (k == kh_end(h));
        if (!missing) {
            return kh_val(h, k);
        }
        env=env->parent;
    }
    return NULL;
}

Obj fget_name(Env *env, char *name){
    Var *var = fget_binding(env, name);
    return *(var->ref);
}

Obj fset_name(Env *env, char *name, Obj obj) {
    Var *var = fget_binding(env, name);
    *(var->ref) = obj;
    return obj;
}

