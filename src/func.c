#include "fvm.h"

/*
 * 
 * */
// OFunc
OFunc* fnew_func(Proto *proto, Env* penv){
    // init all the ovars when a OFunc is created
    OFunc *func = fvm_alloc(OFunc);
    func->obasic.type = T_FUNC;
    func->proto = proto;
    func->penv  = penv;
    return func;
}

Obj fcall(OFunc* func, int argc) {
    Proto   *proto = func->proto;
    Env     *penv  = func->penv;

    if (argc < proto->c_params) {
        fvm_panic("ApplyError: OFunc:<0x%lx> params do not match. (%d of %d)", (Addr)func, argc, proto->c_params);
    }

    // make a closure
    Env *env = fnew_env(proto, penv);

    // parameters are the top lvars
    // pop params into lvars
    int i;
    for(i=0;i<argc;i++){
        fset_local(env, i, fpop());
    }
    
    Obj r = fvm_run(proto, env);

    // TODO: delay cleaning job to GC!
    // fdel_env(env);

    return r;
}


