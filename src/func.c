#include "fvm.h"

/*
 * 
 * */
// OFunc
OFunc* fnew_func(Proto *proto, Env* penv){
    OFunc *func = fvm_alloc(OFunc);
    func->obasic.type = T_FUNC;
    func->proto = proto;
    func->penv  = penv;
    return func;
}

CFunc* fnew_cfunc(ccall_t *fp, size_t c_params){
    CFunc *cfunc = fvm_alloc(CFunc);
    cfunc->obasic.type = T_CFUNC;
    cfunc->fp       = fp;
    cfunc->c_params = c_params;
    return cfunc;
}

// while evaluation ends, No needs clean the env
// it's the job of GC
Obj fcall(Obj obj, int argc) {
    // if cfunc
    if (ftype_of(obj) == T_CFUNC) {
        Obj argv[NPARAMS];
        int i;
        for(i=0; i<argc; i++){
            argv[i] = fpop();
        }
        CFunc   *cfunc = (CFunc *) obj; 
        ccall_t *ccall = &(cfunc->fp);
        Obj r = (*ccall)(argc, argv);
        fpush(r);
        return r;
    }
    // if ofunc
    else if (ftype_of(obj) == T_FUNC) {
        OFunc *func  = (OFunc *) obj; 
        Proto *proto = func->proto;
        Env   *penv  = func->penv;

        if (argc < proto->c_params) {
            fvm_panic("ApplyError: OFunc:<0x%lx> params do not match. (%d of %d)", (Addr)func, argc, proto->c_params);
        }
        // make a closure
        Env *env = fnew_env(proto, penv);
        // arguments are the top lvars
        // pop params into lvars
        int i;
        for(i=0;i<argc;i++){
            fset_local(env, i, fpop());
        }
        // opcodes evaluation
        Obj r = fvm_run(proto, env);
        return r;
    }
    else {
        fvm_panic("TypeError: Obj<0x%lx> not a func", (Addr)obj);
        return fnil();
    }
}


