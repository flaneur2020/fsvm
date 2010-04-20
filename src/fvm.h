#ifndef fvm_h
#define fvm_h

#include <stdio.h>
#include <stdlib.h>

#include <gc.h>
#include "vendor/khash.h"

#define FDEBUG 1

// MAX and blah~
#define NSTACK   1024
#define NPROTOS  512
#define NCONSTS  256
#define NLOCALS  256
#define NOUTERS  256
#define NPARAMS  32

//just for opcode
typedef short int Op;

// tags for each object, which indicated types
typedef enum {
    T_NIL=0,
    T_UNDEF,
    T_NUM,
    T_SYM,
    T_STR,
    T_FUNC,
    T_CFUNC
} Tag;

#define FVM_TAG_NAMES \
    "t_nil", \
    "t_undef", \
    "t_num", \
    "t_sym", \
    "t_str", \
    "t_func" \
    "t_cfunc" \

typedef unsigned long Addr;
typedef unsigned long Obj;

#define T ftype_of

#define Vnil    0
#define Vundef  2

#define Vfunc(o) ((OFunc *)(o))
#define Vstr(o)  (((OStr *)o)->cstr)
#define Vnum(o)  (fto_cint(o))

// TODO: type about
typedef struct Type {
    char                *name;
    //(char *)            (*to_str)(struct Obj);
} Type;

//Obj about
//Obj Header, every obj has got this
typedef struct OBasic {
    unsigned long       type;
} OBasic;

#define OFLAG_NUM 0x01

typedef struct OStr {
    OBasic              obasic;
    char                *cstr;  
    size_t              len;
} OStr;

// all obj stores in the heap
// so easier to implement the closure
// 
// var
typedef struct {
    char        *name;
    Obj         *ref;
} Var;

// hash data type initd
KHASH_MAP_INIT_STR(str, Var*);

// TODO: 
//  when a new Proto is created, 
//  lexer should find all names inside it.
//  parameters are the top local_names
typedef struct Proto {
    Op                  *opcodes;
    Obj                 consts[NCONSTS];
    size_t              c_consts;
    size_t              c_params;
    // for closure
    char                *lnames[NLOCALS];
    char                *onames[NOUTERS];
    size_t              c_lvars;
    size_t              c_ovars;
} Proto;

// only one stack inside a VM
// root is just the global
typedef struct VM {
    Obj                 *stack;
    Obj                 *sp;
    Proto               **protos;
    size_t              c_protos;
    struct Env          *root;
} VM;

// TODO:
//  each Var have got a name, and stored in lvars[]
//  when a OFunc inited, each var->obj inited with Fnil (memset?) as default 
//  if a var is accessed, it will seek obj* in h_locals, and cache it
//
//  when a OFunc dead, it will tranverse all its children
//  if posibble, pass all the values of outer_names to children
typedef struct Env {
    VM                  *vm;
    Proto               *proto;
    khash_t(str)        *h_locals;
    Var                 *lvars;
    Var                 *ovars;
    size_t              c_lvars;
    size_t              c_ovars;
    struct Env          *penv;
} Env;

// proto: stores the codes, names, and blah~ statics things
// penv:  env of parent, NOT the env of function evaluation
typedef struct OFunc {
    OBasic              obasic;
    Proto               *proto;
    Env                 *penv;
} OFunc;

// TODO: make a cfunc
typedef Obj (*ccall_t) (size_t, Obj*);
typedef struct CFunc {
    OBasic              obasic;
    ccall_t             *fp;
    size_t              c_params;
} CFunc;


// macros on gc
#define fvm_malloc   GC_MALLOC
#define fvm_alloc(N) (N*)GC_MALLOC(sizeof(N))
#define fvm_realloc  GC_REALLOC
#define fvm_free  GC_FREE

// print a message & die; in fprintf style
#define fvm_panic(fmt, ...) do{ fprintf(stderr, fmt, ##__VA_ARGS__); exit(1); }while(0);

VM*         fvm_init     ();
VM*         fvm_current  ();

// OFunc is with Env ALWAYS
Env*        fnew_env     (Proto*, Env *);
Obj         fget_local   ();
Obj*        fset_local   ();
Obj         fget_outer   ();
Obj         fset_outer   ();
Var*        freg_binding ();
Var*        fget_binding ();
Obj         fget_name    ();
Obj         fset_name    ();

Proto*      fnew_proto   ();
int         freg_const   ();
Obj         fget_const   ();
int         freg_lname   ();
int         freg_oname   ();

int         freg_proto   ();

OFunc*      fnew_func    ();

// values
int     ftype_of        (Obj);
Obj     fnil            ();
Obj     fundef          ();
Obj     fnum            (int);
Obj     fstr            (char*);
//Obj     ffunc           (OFunc*);
int     fis_nil         (Obj);
int     fnot_nil        (Obj);
int     feq             (Obj,Obj);
int     fgt             (Obj,Obj);
int     flt             (Obj,Obj);
int     fis_str         (Obj);
int     fis_func        (Obj);
int     fis_num         (Obj);
Obj     fto_str         (Obj);
char*   fto_cstr        (Obj);

Obj     fio_puts        (Obj);

/**  main stack **/
Obj  fpush       (Obj);
Obj  fpop        ();

/**  execution and calling **/
// call a function; function is an Obj
Obj fcall           ();
Obj fvm_run         ();

// for debug
//void __print_stack  (Obj *stack, Obj *sp);

char*   _dbg_tag_names(int);
char*   _dbg_obj(Obj);
int     _dbg_show_proto(Proto*);
int     _dbg_show_env(Env*);


#endif
