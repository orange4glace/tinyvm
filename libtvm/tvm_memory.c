#include <tvm/tvm_memory.h>

#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 17

struct tvm_mem *tvm_mem_create(size_t size)
{
	struct tvm_mem *m =
		(struct tvm_mem *)calloc(1, sizeof(struct tvm_mem));

	m->mem_space_size = size;
	m->mem_space = (int *)calloc(size, 1);
  m->total_stack_size = 0;
  m->mutex = PTHREAD_MUTEX_INITIALIZER;
  m->heap_size = 0;

	return m;
}

void tvm_mem_destroy(struct tvm_mem *m)
{
	free(m->mem_space);
	// free(m->registers);
	free(m);
}

int32_t* tvm_mem_allocate_stack(struct tvm_mem *m, size_t size) {
  pthread_mutex_lock(&m->mutex);
  int32_t *ptr = ((int32_t *)m->mem_space) + (m->mem_space_size / sizeof(int32_t))
      - (m->total_stack_size / sizeof(int32_t));
  m->total_stack_size += size;
  pthread_mutex_unlock(&m->mutex);
  return ptr;
}
