#include "fvm.h"

char* _dbg_tag_names(int t){
   static const char fvm_tag_names  [100][100] = { FVM_TAG_NAMES }; 
   return fvm_tag_names[t];
}
char* _dbg_obj(Obj obj){
    printf("%6s | %s", _dbg_tag_names(T(obj)), f2_cstr(obj));
}
int _dbg_show_proto(Proto *proto){
    int i;
    printf("Proto<0x%lx>: \n", (Addr)proto);
    printf("\tparams: %d\n", proto->c_params);
    printf("\tlocals: %d", proto->c_locals);
    printf("\t{ ");
    for(i=0; i<proto->c_locals; i++){
        printf("%s, ", proto->lnames[i]); 
    }
    printf("}\n");
    printf("\touters: %d", proto->c_outers);
    printf("\t");
    for(i=0; i<proto->c_outers; i++){
        printf("%s, ", proto->onames[i]); 
    }
    printf("\n");
    printf("\tconsts: %d\n", proto->c_consts);
    for (i=0;i<proto->c_consts;i++){
        printf("\t\t%3d | %s\n", i, _dbg_obj(fget_const(proto, i)));
    }
    return 0;
}
int _dbg_show_env(Env *env){
    
}

