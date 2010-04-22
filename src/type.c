#include <stdio.h>
#include <string.h>

#include "fvm.h"

// all about primitive types

// private
// just a helper
static Obj fto_str_common(char *fmt, Obj o){
    char tmp[255];
    sprintf(tmp, fmt, o);
    return fstr(tmp);
}

static Obj fto_str_str      (Obj o) { return o; } 
static Obj fto_str_nil      (Obj o) { return fstr("nil"); }
static Obj fto_str_undef    (Obj o) { return fstr("undef"); } 
static Obj fto_str_num      (Obj o) { fto_str_common("%ld", Vnum(o)); } 
static Obj fto_str_func     (Obj o) { fto_str_common("func:<0x%lx>", o); }
static Obj fto_str_cfunc    (Obj o) { fto_str_common("cfunc:<0x%lx>", o); }

// Public 
Type* fnew_type(char *name, to_str_t *to_str){
    Type *type = fvm_alloc(Type);
    strcpy(type->name, name);
    type->to_str = to_str;
    return type;
}

//Tnil, Tundef and blah are all globals
// TODO: make them macro to a VM object
int fvm_init_types(){
    Tnil    = fnew_type("t_nil",    &fto_str_nil);
    Tundef  = fnew_type("t_undef",  &fto_str_undef);
    Tnum    = fnew_type("t_num",    &fto_str_num);
    Tstr    = fnew_type("t_str",    &fto_str_str);
    Tfunc   = fnew_type("t_func",   &fto_str_func);
    Tcfunc  = fnew_type("t_cfunc",  &fto_str_cfunc); 
}
