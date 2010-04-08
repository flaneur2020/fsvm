#include "fvm.h"

int ftype_of(Obj obj){
    if (obj & OFLAG_NUM) {
        return T_NUM;
    }
    if (obj == Vnil) {
        return T_NIL;
    }
    else {
        OBasic *ob = (OBasic *) obj;
        return ob->type;
    }
}

Obj fnil(){
    Obj o = 0;
    return o;
}

Obj fnum(int num){
    // tagged pointer
    Obj o = num<<1 | OFLAG_NUM;
    return o;
}

Obj fstr(char *str) {
    int len  = strlen(str);
    FStr *fstr = (FStr *)fvm_malloc(sizeof(FStr));
    char *cstr = (char *)fvm_malloc(sizeof(char)*len);
    strcpy(cstr, str);
    //
    fstr->obasic.type = T_STR;
    fstr->cstr  = cstr;
    fstr->len   = len;
    Obj o = (Addr) fstr;
    return o;
}

Obj ffunc(Func* func){
    func->obasic.type = T_FUNC;
    Obj o = (Addr) func;
    return o;   
}

int feq(Obj a, Obj b) {
    if (T(a)!=T(b)) {
        return 0;
    }
    switch (ftype_of(a)) {
    case T_NUM:
    case T_SYM: {
                    return (a==b);
    } break;
    case T_STR: {
                    return (strcmp(Vstr(a), Vstr(b))==0);
    } break;
    default:
                    return 0;
    }
}

int fgt(Obj a, Obj b) {
    if (T(a)!=T_NUM || T(b)!=T_NUM){
        fvm_panic("OpError: gt: only t_num can be compared yet");
    }
    return (Vnum(a) > Vnum(b));
}

int flt(Obj a, Obj b) {
    if (T(a)!=T_NUM || T(b)!=T_NUM){
        fvm_panic("OpError: lt: only t_num can be compared yet");
    }
    return (Vnum(a) < Vnum(b));
}

int fis_nil(Obj o) {
    return o==0;
}

int fnot_nil(Obj o) {
    return !fis_nil(o);
}

char* fto_cstr(Obj o) {
    char* str = fvm_malloc(255*sizeof(char));
    switch(T(o)) {
    case T_NIL: 
        sprintf(str, "nil");
        break;
    case T_STR:
        strcpy(str, Vstr(o));
        break;
    case T_NUM:
        sprintf(str, "%ld", Vnum(o));
        break;
    case T_FUNC:
        sprintf(str, "func: 0x%lx", o);
        break;
    default:
        sprintf(str, "unkown type");
        break;
    }
    return str;
}

Obj fto_str(Obj o) {
    char *str = fto_cstr(o);
    Obj ostr = fstr(str);
    return ostr;
}

Obj fio_puts(Obj o){
    printf("%s\n", fto_cstr(o));
    return fnil();
}



