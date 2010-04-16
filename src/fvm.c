#include <stdio.h>
#include <string.h>

#include "fvm.h"


// vm is global
static VM *vm=NULL;
VM* fvm_init() {
    static VM     _vm;
    static Proto  *protos[NPROTOS];
    static Obj    stack[NSTACK];
    if (vm==NULL){
        GC_INIT();
        _vm.stack=stack;
        _vm.sp=stack;
        _vm.protos=protos;
        _vm.c_protos=0;
        //init the first env as root
        _vm.root=fvm_alloc(Env);
        _vm.root->h_locals=kh_init(str);
        _vm.root->vm=&_vm;
        vm=&_vm;
    }
    return vm;
}

VM* fvm_current() {
    return vm;
}
