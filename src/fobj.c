#include "fvm.h"

Obj fnum(int num){
    Obj obj = { T_NUM, num };
    return obj;
}

Obj fstr(char *str) {
    int len = strlen(str);
    char *_str = (char *)fvm_malloc(sizeof(char)*len+10);
    strcpy(_str, str);
    Obj obj = { T_STR, (Addr)_str };
    return obj;
}

Obj ffunc(Func* func){
    Obj obj = { T_FUNC, (Addr)func };
    return obj;   
}

int feq(Obj a, Obj b) {
    if (T(a)!=T(b)) {
        return 0;
    }
    switch (T(a)) {
    case T_NUM:
    case T_SYM: {
                    return (V(a)==V(b));
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

int fis_nil(Obj obj) {
    return (T(obj)==T_SYM && V(obj)==Vnil);
}

int fnot_nil(Obj obj) {
    return !fis_nil(obj);
}

int f2_cint(Obj obj) {
    return (int)obj.val;
}

char* f2_cstr(Obj obj) {
    char* str = fvm_malloc(255*sizeof(char));
    switch(T(obj)) {
    case T_SYM:
        switch(V(obj)) {
        case (Vnil):
            sprintf(str, "nil");
            break;
        default:
            sprintf(str, "sym:%ld", V(obj));
            break;
        }
        break;
    case T_STR:
        strcpy(str, (char *)obj.val);
        break;
    case T_NUM:
        sprintf(str, "%ld", obj.val);
        break;
    case T_FUNC:
        sprintf(str, "func: 0x%lx", obj.val);
        break;
    default:
        sprintf(str, "unkown type");
        break;
    }
    return str;
}

Obj f2_str(Obj obj) {
    char *str = f2_cstr(obj);
    Obj ostr = fstr(str);
    return ostr;
}

Obj fio_puts(Obj obj){
    printf("%s\n", f2_cstr(obj));
    return Fnil;
}



