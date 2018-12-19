#include <tvm/tvm_thread.h>
#include <tvm/tvm_stack.h>
#include <tvm/tvm.h>

#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 17

int next_tvm_thread_id = 0;

struct tvm_thread *tvm_thread_create(struct tvm_ctx *vm)
{
	struct tvm_thread *thread =
		(struct tvm_thread *)calloc(1, sizeof(struct tvm_thread));

  thread->id = next_tvm_thread_id++;
  thread->vm = vm;
	thread->registers = calloc(NUM_REGISTERS, sizeof(union tvm_reg_u));

  tvm_stack_create(vm->mem, thread, MIN_STACK_SIZE);

	return thread;
}

void *__tvm_thread_start(void *param) {
  struct tvm_thread *thread = param;

  int *instr_idx = &thread->registers[0x8].i32;
	for (; thread->vm->prog->instr[*instr_idx] != -0x1; ++(*instr_idx)) {
		tvm_step(thread->vm, thread, instr_idx);
  }
  return 0;
}

void tvm_thread_start(struct tvm_ctx* vm, struct tvm_thread *thread) {
  printf("tvm_thread_start\n");
  pthread_create(&thread->pthread, NULL, &__tvm_thread_start, (void *)thread);
}

void tvm_thread_join(struct tvm_ctx* vm, struct tvm_thread *thread) {
  printf("tvm_thread_join\n");
  int status;
  pthread_join(thread->pthread, (void **)&status);
  printf("tvm_thread_joined\n");
}

void tvm_thread_set_instruction_pointer(struct tvm_thread *thread, int ip) {
  int *instr_idx = &thread->registers[0x8].i32;
  *instr_idx = ip;
}
