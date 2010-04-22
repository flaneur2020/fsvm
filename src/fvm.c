#include <stdio.h>
#include <string.h>

#include "fvm.h"

// vm is global
static VM *vm=NULL;
VM* fvm_init() {
    static VM _vm;
    if (vm==NULL){
        GC_INIT();
        // init the structure of _vm
        _vm.sp          = _vm.stack;
        _vm.c_protos    = 0;
        _vm.c_types     = 0;
        //TODO: init the top env as global
        _vm.root=fvm_alloc(Env);
        _vm.root->penv=NULL;
        _vm.root->h_locals=kh_init(str);
        _vm.root->vm=&_vm;
        vm=&_vm;

        fvm_init_types();
    }
    return vm;
}

VM* fvm_current() {
    return vm;
}




