#include "fvm.h"

int ftype_of(Obj o){
    if (o & OFLAG_NUM) {
        return T_NUM;
    }
    if (o == Vnil) {
        return T_NIL;
    }
    else {
        OBasic *obj = (OBasic *) o;
        return obj->type;
    }
}

int T(Obj o) {
    return ftype_of(o);
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
    return (f2_cint(a) > f2_cint(b));
}

int flt(Obj a, Obj b) {
    if (T(a)!=T_NUM || T(b)!=T_NUM){
        fvm_panic("OpError: lt: only t_num can be compared yet");
    }
    return (f2_cint(a) < f2_cint(b));
}

int fis_nil(Obj o) {
    return o==0;
}

int fnot_nil(Obj o) {
    return !fis_nil(o);
}

int f2_cint(Obj o) {
    return (int)(o>>1);
}

char* f2_cstr(Obj o) {
    char* str = fvm_malloc(255*sizeof(char));
    switch(T(o)) {
    case T_SYM:
        switch(o) {
        case (Vnil):
            sprintf(str, "nil");
            break;
        default:
            sprintf(str, "sym:%ld", o);
            break;
        }
        break;
    case T_STR:
        strcpy(str, ((FStr *)o)->cstr);
        break;
    case T_NUM:
        sprintf(str, "%ld", f2_cint(o));
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

Obj f2_str(Obj o) {
    char *str = f2_cstr(o);
    Obj ostr = fstr(str);
    return ostr;
}

Obj fio_puts(Obj o){
    printf("%s\n", f2_cstr(o));
    return fnil();
}



