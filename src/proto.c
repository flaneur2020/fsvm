#include "fvm.h"

/*
 * 
 * */

// Proto
Proto* fnew_proto(Op *opcodes, int c_params) {
    Proto *proto = fvm_alloc(Proto);
    proto->opcodes = opcodes;
    proto->c_params = c_params;
    
    return proto;
}

int freg_proto(VM *vm, Proto *proto){
    int pid = vm->c_protos;
    vm->protos[pid]=proto;
    vm->c_protos++;
    return pid;
}

Proto* fget_proto(VM *vm, int pid){
    if (pid > vm->c_protos || pid < 0){
        fvm_panic("ProtoError: Uncaught Proto: %d", pid);
    } 
    Proto* proto = vm->protos[pid];
    return proto;
}

// fields of Proto
int freg_const(Proto *proto, Obj obj){
    int cid = proto->c_consts;
    proto->consts[cid]=obj;
    proto->c_consts++;
    return cid;
}

Obj fget_const(Proto *proto, int cid){
    if (cid >= proto->c_consts) {
        fvm_panic("ProtoError: consts[%d] do not exists\n", cid);
    }
    return proto->consts[cid];
}

// local_name
int freg_lname(Proto *proto, char *str) {
    int id = proto->c_lvars;
    proto->lnames[id] = str;
    proto->c_lvars++;
    return id;
}

//outer_name
int freg_oname(Proto *proto, char *str) {
    int id = proto->c_ovars;
    proto->onames[id] = str;
    proto->c_ovars++;
    return id;
}
