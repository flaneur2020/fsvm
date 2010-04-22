#include "fvm.h"

char* _dbg_obj(Obj obj){
    printf("%6s | %s\n", (ftype_of(obj))->name, fto_cstr(obj));
}
char* _dbg_2str(Obj obj){
    static char str[255];
    char *vstr = fto_cstr(obj);
    sprintf(str, "%6s | %s\n", (ftype_of(obj))->name, vstr);
    return str;
}

int _dbg_show_proto(Proto *proto){
    int i;
    printf("Proto<0x%lx>: \n", (Addr)proto);
    printf("\tparams: %d\n", proto->c_params);
    printf("\tlocals: %d", proto->c_lvars);
    printf("\t");
    for(i=0; i<proto->c_lvars; i++){
        printf("%s, ", proto->lnames[i]); 
    }
    printf("\n");
    printf("\touters: %d", proto->c_ovars);
    printf("\t");
    for(i=0; i<proto->c_ovars; i++){
        printf("%s, ", proto->onames[i]); 
    }
    printf("\n");
    printf("\tconsts: %d\n", proto->c_consts);
    for (i=0;i<proto->c_consts;i++){
        printf("\t\t %d : %s", i, _dbg_2str(proto->consts[i]));
    }
    return 0;
}
int _dbg_show_env(Env *env){
    
}

