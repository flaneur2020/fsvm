#include <stdio.h>
#include <string.h>

#include "fvm.h"


// Env do NOT need a parent env!
Env* fnew_env(Proto *proto, Env *penv) {
    Env* env  = fvm_alloc(Env);
    env->vm   = fvm_current();
    env->penv     = penv;
    env->h_locals = kh_init(str);
    env->proto    = proto;
    
    if (proto==NULL) {
        return env;
    }

    // allocate all the Vars and Objs, 
    // and do some initializtions
    size_t c_lvars = env->c_lvars = proto->c_lvars;
    size_t c_ovars = env->c_ovars = proto->c_ovars;
    env->lvars = fvm_malloc( c_lvars * sizeof(Obj *) );
    int i;
    for(i=0; i<c_lvars; i++) {
        Var *lvar = &(env->lvars[i]);
        Obj *ref  = fvm_alloc(Obj);
        *ref = fundef();
        lvar->name   = proto->lnames[i];
        lvar->ref    = ref;
        freg_binding(env, lvar);
    }

    // init all the ovars in the hash
    env->ovars = fvm_malloc( c_ovars * sizeof(Var) );
    for(i=0; i<c_ovars; i++){
        char *name = proto->onames[i];
        Var  *ovar  = fget_binding(penv, name);
        if (ovar == NULL) {
            fvm_panic("NameError: '%s' not found\n", name);
        }
        env->ovars[i] = *ovar;
    }
    return env;
}

Env* froot_env(){
    return NULL;
}

//you know
int fdel_env(Env *env){
    kh_destroy(str, env->h_locals);
    fvm_free(env->lvars);
    fvm_free(env);
    return 0;
}

Obj fget_local(Env *env, int id) {
    Var *v = &env->lvars[id];
    return *(v->ref);
}

Obj* fset_local(Env *env, int id, Obj obj) {
    //TODO: add boundary check 
    Var *v = &(env->lvars[id]);
    *(v->ref) = obj;
    return v->ref;
}

Obj fget_outer(Env *env, int id) {
    Var *v = &(env->ovars[id]);
    return *(v->ref);
}

Obj fset_outer(Env *env, int id, Obj obj){
    Var *v = &(env->ovars[id]);
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
    Env *cenv = env;
    while (cenv!=NULL) {
        khash_t(str) *h; khiter_t k; int missing;
        h=env->h_locals;
        k=kh_get(str, h, name);
        missing = (k == kh_end(h));
        if (!missing) {
            return kh_val(h, k);
        }
        cenv = cenv->penv;
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

