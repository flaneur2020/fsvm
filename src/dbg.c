#include "fvm.h"

char* _dbg_tag_names(int t){
   static const char fvm_tag_names  [100][100] = { FVM_TAG_NAMES }; 
   return fvm_tag_names[t];
}
char* _dbg_obj(Obj obj){
    char str[255];
    sprintf(str, "%6s | %s", _dbg_tag_names(T(obj)), f2_cstr(obj));
    return str;
}
int _dbg_show_proto(Proto *proto){
    printf("Proto<0x%lx>: \n", (Addr)proto);
    printf(" Consts: %d\n ", proto->c_consts);
    int i;
    for (i=0;i<proto->c_consts;i++){
        printf("\t%3d | %s\n", i, _dbg_obj(fget_const(proto, i)));
    }
    return 0;
}
int _dbg_show_env(Env *env){
    
}

