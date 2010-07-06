#ifndef fvm_h
#define fvm_h

#include <stdio.h>
#include <stdlib.h>

#include <gc.h>
#include "vendor/khash.h"

#define FDEBUG 1

/* some MAX constants in struct VM */

// Max in struct Env
#define NSTACK   1024
#define NPROTOS  512
#define NTYPES   128
// Max in struct Proto
#define NCONSTS  256
#define NLOCALS  256
#define NOUTERS  256
#define NPARAMS  32

//just for opcode
typedef short int Op;

typedef unsigned long Addr;
typedef unsigned long Obj;

/*
 * All the *Type* stuff. Maybe a new .h file is needed, but I'm a lazy man.
 * 
 * T(o) return the type of an object. Each type can be parsed *to_str*, and 
 * make compair by *eq*.
 *
 * Vstr(o) and Vnum(o) blah~ can make Obj into c types. 
 * NOTE: do not have type check right now.
 *
 * Currently, Fsvm give 6 types and do not have support of user type. So, all
 * types are initialized in type.c(fvm_init_types()).
 *
 * Obj of Tnil, Tundef and Tnum are 'passed by value'. The rest are 'passed by 
 * reference'.
 *
 * */

#define T ftype_of

#define Vnil    0
#define Vundef  2

#define Vstr(o)  (((OStr *)o)->cstr)
#define Vnum(o)  (fto_cint(o))

typedef Obj (*to_str_t) (Obj);
typedef int (*eq_t) (Obj, Obj);

// type about
typedef struct Type {
    char                name[255];
    to_str_t            *to_str;
    eq_t                *eq;
} Type;

Type *Tnil; 
Type *Tundef; 
Type *Tnum;
Type *Tstr;
Type *Tfunc;
Type *Tcfunc;

#define FVM_TAG_NAMES \
    "t_nil", \
    "t_undef", \
    "t_num", \
    "t_str", \
    "t_func" \
    "t_cfunc" \

/*
 * OBasic is the common stuff of every Obj. In OO view, all the Obj inherit from it.
 *
 *     OStr *ostr = fstr("str");
 *     OBasic *ob = (OBasic *)ostr;
 *     ob->type....
 *
 * Polymorphism is Egghurting.
 *
 * */

typedef struct OBasic {
    Type                *type;
} OBasic;

// Number in Fsvm is a *tagged pointer*
#define OFLAG_NUM 0x01

typedef struct OStr {
    OBasic              obasic;
    char                *cstr;  
    size_t              len;
} OStr;

/*
 * all obj stores in the heap, easier to implement closure
 *
 * */ 
typedef struct Var {
    char        *name;
    Obj         *ref;
} Var;

// khash inited. khash_t(str)
KHASH_MAP_INIT_STR(str, Var*);

// TODO: 
//  when a new Proto is created, lexer should find all names inside it.
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

/*
 * VM is a singleton(right now). It stores the global executation *stack*, *sp*, all the 
 * *types* and *protos*. 
 *
 */
typedef struct VM {
    Obj                 stack[NSTACK];
    Obj                 *sp;
    Type                *types[NTYPES];
    Proto               *protos[NPROTOS];
    size_t              c_types;
    size_t              c_protos;
    struct Env          *root;
} VM;

/*
 * An Env save the current state (local variables, outer variable) of opcode executation. 
 * If one function is a closure, it must have a penv(Parent Env).
 *
 * While a function(ofunc) is being called, an Env is created. Function can be created while 
 * opcode executation, by the instruction OP_MKFUNC. It take one parameter n, which indicated 
 * a Proto number, and save the pointer of current Env as penv, make a new function. 
 *
 * Each Var have got a name, and referenced in *lvars[]* and *h_locals*. *h_locals* is a hash table, 
 * which char*->Var. Mostly, when a OFunc is initialized, it will get all the outer variables'
 * name, and put all the binding(by `fget_binding()`) inside *ovars[]*.
 *
 * All the value of *lvars* are initialized as Vundef. The first few lvars are the parameters 
 * of this function calling.
 *
 * When a function calling is over, an Env still lives until the Boehm GC collect it. 
 *
 */

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
#define fvm_panic(fmt, ...) do{ fprintf(stderr, fmt, ##__VA_ARGS__); exit(1); }while(0)

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

int         freg_proto   (Proto *);
Proto*      fget_proto   (int);

OFunc*      fnew_func    (Proto*, Env*);
CFunc*      fnew_cfunc   (ccall_t *, size_t);

// values
Type*   ftype_of        (Obj);
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
int     fto_cint        (Obj);
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
