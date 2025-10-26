#include <syscalls.h>
#include <status.h>
#include <video.h>

uint64_t syscallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6){
    switch(syscall_id){
        case SYS_WRITE : 
            return textWrite(arg1, (char *) arg2, arg3);
        default :
            return ERROR;
    }
}