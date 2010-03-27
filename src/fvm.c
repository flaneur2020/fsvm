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

Env* fnew_env(Env *parent) {
    Env* env = fvm_alloc(Env);
    env->vm = fvm_current();
    env->h_locals = kh_init(str);
    if (parent==NULL){
        parent = env->vm->root;
    }
    env->parent = parent;
    return env;
}

Obj fget_local(Env *env, int id) {
    Var *v = &env->locals[id];
    //TODO: introduce undefined?
    if (fis_nil(v->obj)) {
        Obj o = fget_name(v->name);
        v->obj = o;
    }
    return v->obj;
}

Obj* fset_local(Env *env, int id, Obj obj) {
    //TODO: add boundary check 
    Var *v = &(env->locals[id]);
    v->obj = obj;
    return &(v->obj);
}

Obj fget_outer(Env *env, int id) {
    OVar *v = &(env->outers[id]);
    if (v->ref==NULL) {
        Obj *ref = fget_name_ref(env, v->name);
        v->ref = ref;
    }
    return *(v->ref);
}

Obj fset_outer(Env *env, int id, Obj obj){
    OVar *v = &(env->outers[id]);
    if (v->ref==NULL) {
        Obj *ref = fget_name_ref(env, v->name);
        v->ref = ref;
    }
    *(v->ref) = obj;
    return *(v->ref);
}

Obj fbind_name(Env *env, char *name, Obj *obj_ref) {
    khiter_t k; int ret;
    khash_t(str) *h=env->h_locals;
    k=kh_put(str, h, name, &ret);
    if (!ret){
        kh_del(str, h, k);
    }
    kh_val(h, k) = obj_ref;
    return *obj_ref;
}

Obj* fget_name_ref(Env *env, char *name) {
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
    Obj *ref = fget_name_ref(env, name);
    if (ref == NULL) {
        return fnil();
    }
    return *ref;
}

Obj fset_name(Env *env, char *name, Obj *obj_ref) {
    while (env != NULL){
        khash_t(str) *h; khiter_t k; int missing;
        h=env->h_locals;
        k=kh_get(str, h, name);
        missing = (k == kh_end(h));
        if (!missing) {
            kh_val(h, k) = obj_ref;
            return *obj_ref;
        }
        env=env->parent;
    }
    return fnil();
}

