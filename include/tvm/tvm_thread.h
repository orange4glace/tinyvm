#ifndef TVM_THREAD_H_
#define TVM_THREAD_H_

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include "tvm_memory.h"

struct tvm_ctx;

struct tvm_thread {
  
  struct tvm_ctx *vm;
  pthread_t pthread;

	int FLAGS;
	int remainder;

	union tvm_reg_u *registers;

};

struct tvm_thread *tvm_thread_create(struct tvm_ctx *vm);
void tvm_thread_start(struct tvm_ctx* vm, struct tvm_thread *thread);
void tvm_thread_join(struct tvm_ctx* vm, struct tvm_thread *thread);
void tvm_thread_set_instruction_pointer(struct tvm_thread *thread, int ip);

#endif
