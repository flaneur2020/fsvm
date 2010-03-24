#ifndef fvm_h
#define fvm_h

#include <stdio.h>
#include <stdlib.h>

#include <gc.h>
#include "vendor/khash.h"

#define FDEBUG 1

//just for opcode
typedef short int Op;

// tags for each object, it indicated their types
typedef enum {
    T_SYM=0,
    T_NUM,
    T_STR,
    T_FUNC
} Tag;

#define FVM_TAG_NAMES \
    "t_sym", \
    "t_num", \
    "t_str", \
    "t_func" \

typedef unsigned long Addr;

typedef struct {
    Tag             tag;
    Addr            val;
} Obj;

enum {
    Vnil = 0
};

#define T(o) ((o).tag)
#define V(o) ((o).val)
#define Vfunc(o) ((Func *)((o).val))
#define Vstr(o) ((char *)((o).val))
#define Vnum(o) ((int)((o).val))
#define Fnil ((Obj){0,0})

typedef struct {
    char        *name;
    Obj         obj;
} Var;

// hash data type initd
KHASH_MAP_INIT_STR(str, Obj*);

// TODO: 
//  when a new Proto is created, 
//  lexer should find all names inside it.
//  parameters are the top local_names
typedef struct Proto {
    Op                  *opcodes;
    Obj                 consts[255];
    size_t              c_consts;
    size_t              c_params;
    // for closure
    char                **local_names;
    char                **outer_names;
    size_t              c_local_names;
    size_t              c_outer_names;
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
//  each Var have got a name, and stored in locals[]
//  when a Func inited, each var->obj inited with Fnil (memset?) as default 
//  if a var is accessed, it will seek obj* in h_locals, and cache it
//
//  when a Func dead, it will tranverse all its children
//  if posibble, pass all the values of outer_names to children
typedef struct Env {
    VM                  *vm;
    khash_t(str)        *h_locals;
    Var                 locals[255];
    Obj                 *base;
    struct Env          *parent;
    struct Env          *children;
} Env;

// TODO:
typedef struct Func {
    Proto               *proto;
    Env                 *env;
} Func;



// macros on gc
#define fvm_malloc   GC_MALLOC
#define fvm_alloc(N) (N*)GC_MALLOC(sizeof(N))
#define fvm_realloc  GC_REALLOC
#define fvm_free(S)  UNUSED(S)

// print a message & die; in fprintf style
#define fvm_panic(fmt, ...) do{ fprintf(stderr, fmt, ##__VA_ARGS__); exit(1); }while(0);

VM*         fvm_init     ();
VM*         fvm_current  ();

// create a new Context when a Function is just being executed
Env*        fnew_env     ();
Obj         fbind_var    ();
Obj         fget_var     ();
Obj         fset_var     ();
Obj         fget_tmp     ();
Obj         fset_tmp     ();

Proto*      fnew_proto   ();
int         fset_const   ();
Obj         fget_const   ();

Func*       fnew_func    ();

// values
Obj     fnum            (int);
Obj     fstr            (char*);
Obj     ffunc           (Func*);
int     fis_nil         (Obj);
int     fnot_nil        (Obj);
int     feq             (Obj,Obj);
int     fgt             (Obj,Obj);
int     flt             (Obj,Obj);
int     fis_str         (Obj);
int     fis_func        (Obj);
int     fis_num         (Obj);
Obj     f2_str          (Obj);
char*   f2_cstr         (Obj);
int     f2_cint         (Obj);

Obj     fio_puts        (Obj);

/**  main stack **/
Obj  fpush       (Obj);
Obj  fpop        ();

/**  execution and calling **/
// call a function; function is an Obj
Obj fcall        ();


// for debug
//void __print_stack  (Obj *stack, Obj *sp);

char*   _dbg_tag_names(int);
char*   _dbg_obj(Obj);
int     _dbg_show_proto(Proto*);
int     _dbg_show_env(Env*);


#endif
