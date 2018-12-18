#ifndef TVM_STACK_H_
#define TVM_STACK_H_

#define MIN_STACK_SIZE (2 * 1024 * 1024) /* 2 MB */

#include "tvm_memory.h"
#include "tvm_thread.h"

/* Initialize our stack by setting the base pointer and stack pointer */

static inline void tvm_stack_create(struct tvm_mem *mem, struct tvm_thread *thread, size_t size)
{
	thread->registers[0x7].i32_ptr = tvm_mem_allocate_stack(mem, size);
	thread->registers[0x6].i32_ptr = thread->registers[0x7].i32_ptr;
}

static inline void tvm_stack_push(struct tvm_thread *thread, int *item)
{
	thread->registers[0x6].i32_ptr -= 1;
	*thread->registers[0x6].i32_ptr = *item;
}

static inline void tvm_stack_pop(struct tvm_thread *thread, int *dest)
{
	*dest = *thread->registers[0x6].i32_ptr;
	thread->registers[0x6].i32_ptr += 1;
}

#endif
