#ifndef TVM_MEMORY_H_
#define TVM_MEMORY_H_

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdint.h>
#include <stddef.h>

#define MIN_MEMORY_SIZE (64 * 1024 * 1024) /* 64 MB */

union tvm_reg_u {
	int32_t i32;
	int32_t *i32_ptr;

	union {
		int16_t h;
		int16_t l;
	} i16;

};

struct tvm_mem {
	/*
	 *	Similar to x86 FLAGS register
	 *
	 *	0x1	EQUAL
	 *	0x2	GREATER
	 *
	 */

	void *mem_space;
	int mem_space_size;
  size_t total_stack_size;

  pthread_mutex_t mutex;

  int heap_size;
};

struct tvm_mem *tvm_mem_create(size_t size);
void tvm_mem_destroy(struct tvm_mem *mem);
int32_t* tvm_mem_allocate_stack(struct tvm_mem *m, size_t size);

// allocate memory and return address
static inline int32_t tvm_mem_malloc(struct tvm_mem *mem, size_t size) {
  return mem->heap_size++;
}

#endif
