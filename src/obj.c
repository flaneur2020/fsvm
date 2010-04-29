#include "fvm.h"

Type* ftype_of(Obj obj){
    if (obj & OFLAG_NUM) {
        return Tnum;
    }
    switch(obj) {
    case Vnil:
                return Tnil;
    case Vundef:
                return Tundef;
    default: {
                OBasic *ob = (OBasic *) obj;
                return ob->type;
             }
    }
}

Obj fnil(){
    Obj o = Vnil;
    return o;
}

Obj fundef(){
    Obj o = Vundef;
    return o;
}

// convert into tagged, 30 bit width num?
Obj fnum(int num){
    Obj obj = (Obj) num <<1;
    obj |= 1;
    return obj;
}

int fto_cint(Obj o){
    int num=(int) o;
    num = num >> 1;
    return num;
}

Obj fstr(char *str) {
    int len  = strlen(str);
    OStr *fstr = (OStr *)fvm_malloc(sizeof(OStr));
    char *cstr = (char *)fvm_malloc(sizeof(char)*len+1);
    strcpy(cstr, str);
    //
    fstr->obasic.type = Tstr;
    fstr->cstr  = cstr;
    fstr->len   = len;
    Obj o = (Addr) fstr;
    return o;
}

int feq(Obj a, Obj b) {
    if (a==b) {
        return 1;
    }
    if (T(a)!=T(b)) {
        return 0;
    }
    if (T(a)==Tnum) {
        return (a==b);
    }
    else if (T(a)==Tstr) {
        return (strcmp(Vstr(a), Vstr(b))==0);
    }
    else {
        return 0;
    }
}

int fgt(Obj a, Obj b) {
    if (T(a)!=Tnum || T(b)!=Tnum){
        fvm_panic("OpError: gt: only t_num can be compared yet");
    }
    return (Vnum(a) > Vnum(b));
}

int flt(Obj a, Obj b) {
    if (T(a)!=Tnum || T(b)!=Tnum){
        fvm_panic("OpError: lt: only t_num can be compared yet");
    }
    return (Vnum(a) < Vnum(b));
}

int fis_nil(Obj o) {
    return o==Vnil;
}

int fnot_nil(Obj o) {
    return !fis_nil(o);
}

char* fto_cstr(Obj obj) {
    OStr *ostr = (OStr *)(fto_str(obj));
    return ostr->cstr;
}

Obj fto_str(Obj obj) {
    if (ftype_of(obj)->to_str == NULL) {
        fvm_panic("TypeError: Obj<%s:0x%lx> cannot convert to string\n", ftype_of(obj)->name, (Addr)obj);
    }
    to_str_t *to_str = &(ftype_of(obj)->to_str);
    return  (*to_str)(obj);
}

Obj fio_puts(Obj o){
    printf("%s\n", fto_cstr(o));
    return fnil();
}



