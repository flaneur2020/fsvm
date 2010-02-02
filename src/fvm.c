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
        _vm.root->locals=kh_init(str);
        _vm.root->vm=&_vm;
        vm=&_vm;
    }
    return vm;
}

VM* fvm_current() {
    return vm;
}

// context
Env* fnew_env(Env *parent) {
    Env* env = fvm_alloc(Env);
    env->vm = fvm_current();
    env->locals = kh_init(str);
    if (parent==NULL){
        parent = env->vm->root;
    }
    env->parent = parent;
    return env;
}

Obj fget_tmp(Env *env, int id) {
    return env->tmp[id];
}

Obj fset_tmp(Env *env, int id, Obj obj){
    env->tmp[id]=obj;
    return obj;
}

Obj fbind_var(Env *env, char *name, Obj obj) {
    khiter_t k; int ret;
    khash_t(str) *h=env->locals;
    k=kh_put(str, h, name, &ret);
    if (!ret){
        kh_del(str, h, k);
    }
    kh_val(h, k)=obj;
    return obj;
}

Obj fget_var(Env *env, char *name){
    while (env != NULL){
        khash_t(str) *h; khiter_t k; int missing;
        h=env->locals;
        k=kh_get(str, h, name);
        missing = (k == kh_end(h));
        if (!missing) {
            return kh_val(h, k);
        }
        env=env->parent;
    }
    return Fnil;
}

Obj fset_var(Env *env, char *name, Obj obj) {
    while (env != NULL){
        khash_t(str) *h; khiter_t k; int missing;
        h=env->locals;
        k=kh_get(str, h, name);
        missing = (k == kh_end(h));
        if (!missing) {
            kh_val(h, k) = obj;
            return obj;
        }
        env=env->parent;
    }
    return Fnil;
}

