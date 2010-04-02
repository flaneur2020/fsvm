#include <stdio.h>
#include <string.h>

#include "fvm.h"


// Env do NOT need parent!
Env* fnew_env(Env *from, size_t c_lvars, size_t c_ovars, Var* ovars) {
    Env* env  = fvm_alloc(Env);
    env->vm   = fvm_current();
    env->from = from;
    env->h_locals = kh_init(str);
    env->c_lvars = c_lvars;
    env->c_ovars = c_ovars;
    // allocate all the Vars and Objs, 
    // TODO: memset as 0!
    env->lvars = fvm_malloc( c_lvars * sizeof(Var) );
    int i;
    for(i=0; i<c_lvars; i++) {
        env->lvars[i].name = NULL;
        env->lvars[i].ref  = fvm_alloc(Obj);
    }

    // init all the ovars in the hash
    env->ovars = ovars;
    if (ovars != NULL){
        for(i=0; i<c_ovars; i++){
            freg_binding(env, &ovars[i]);
        }
    }
    return env;
}

//TODO: you know
int fdel_env(Env *env){
    //fvm_free(env->h_locals);
    //int i;
    //for(i=0; i<env->c_lvars; i++){
        //
    //}
    //fvm_free(env->lvars);
    //fvm_free(env->ovars);
    // TODO: make each env->children->parent = env->parent;
    //fvm_free(env);
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
    khash_t(str) *h; khiter_t k; int missing;
    h=env->h_locals;
    k=kh_get(str, h, name);
    missing = (k == kh_end(h));
    if (!missing) {
        return kh_val(h, k);
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

