#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/thread.h"
#include "kernel/riscv.h"

int thread_create(void *(start_routine)(void*), void *arg){
    void* ustack = malloc(PGSIZE);
    if(!ustack){
        return -1;
    }
    // grow downside
    int pid = clone(ustack+PGSIZE);
    if(pid==0){
        // child
        start_routine(arg);
        exit(0);
    }else{
        // parent
        return 0;
    }
    return 0;
}

void lock_init(struct lock_t* lock){
    lock->locked = 0;
}

void lock_acquire(struct lock_t* lock){
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0);
    __sync_synchronize();
}

void lock_release(struct lock_t* lock){
    __sync_synchronize();
    __sync_lock_release(&lock->locked);
}