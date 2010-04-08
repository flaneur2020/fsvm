#include "fvm.h"

/*
 * 
 * */
// OFunc
OFunc* fnew_func(Proto *proto, Env* parent){
    // init all the ovars when a OFunc is created
    int i;
    int c_ovars = proto->c_ovars;
    Var *ovars = fvm_malloc(c_ovars*sizeof(Var));
    for (i=0; i<c_ovars; i++) {
        char *name = proto->onames[i];
        Var  *ovar = fget_binding(parent, name);
        ovars[i] = *ovar;
    }

    OFunc *func = fvm_alloc(OFunc);
    func->obasic.type = T_FUNC;
    func->proto = proto;
    func->ovars = ovars;
    return func;
}

Obj fcall(OFunc* func, int argc, Env *from) {
    Proto   *proto = func->proto;
    if (argc < proto->c_params) {
        fvm_panic("ApplyError: OFunc:<0x%lx> params do not match. (%d of %d)", (Addr)func, argc, proto->c_params);
    }

    // make a closure
    int c_lvars = proto->c_lvars;
    int c_ovars = proto->c_ovars;
    Env *env = fnew_env(from, c_lvars, c_ovars, func->ovars);
    // init all the lvars as nil
    int i;
    for(i=0; i<proto->c_lvars; i++){
        char *name = proto->lnames[i];
        Var  *lvar = &(env->lvars[i]);
        lvar->name  = name;
        fset_local(env, i, fundef());
        freg_binding(env, lvar);
    }

    // parameters are the top lvars
    // pop params into lvars
    for(i=0;i<argc;i++){
        fset_local(env, i, fpop());
    }
    
    // TODO: do some clean here
    // env can be cleaned now . TODO: if refcount were introduced, map (-1) vars.ref
    Obj r = fvm_run(proto, env);
    // clean the env
    fdel_env(env);

    return r;
}


