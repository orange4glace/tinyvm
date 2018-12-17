#ifndef TVM_H_
#define TVM_H_

#include <stddef.h>

#include "tvm_file.h"
#include "tvm_preprocessor.h"
#include "tvm_stack.h"

#include "tvm_memory.h"
#include "tvm_program.h"
#include "tvm_tokens.h"

struct tvm_ctx {
	struct tvm_prog *prog;
	struct tvm_mem *mem;
};

struct tvm_ctx *tvm_vm_create();
void tvm_vm_destroy(struct tvm_ctx *vm);

int tvm_vm_interpret(struct tvm_ctx *vm, char *filename);
void tvm_vm_run(struct tvm_ctx *vm);


#define args_rule(INDEX) (*args[INDEX + 4] == 0 ? \
  (args[INDEX] + *args[INDEX + 2]) : \
  *args[INDEX + 4] == 1 ? \
  ((int32_t*)(size_t)(*args[INDEX]) + *args[INDEX + 2]) : \
  &((int *)vm->mem->mem_space)[*args[INDEX]])

#define args0 args_rule(0)
#define args1 args_rule(1)

// #define args0 (args[0] + *args[2])
// #define args1 (args[1] + *args[3])

static inline void tvm_step(struct tvm_ctx *vm, int *instr_idx)
{
	int **args = vm->prog->args[*instr_idx];

	switch (vm->prog->instr[*instr_idx]) {
/* nop   */	case 0x0:  break;
/* int   */	case 0x1:  /* unimplemented */ break;
/* mov   */	case 0x2:  *args0 = *args1; break;
/* push  */	case 0x3:  tvm_stack_push(vm->mem, args0); break;
/* pop   */	case 0x4:  tvm_stack_pop(vm->mem, args0); break;
/* pushf */	case 0x5:  tvm_stack_push(vm->mem, &vm->mem->FLAGS); break;
/* popf  */	case 0x6:  tvm_stack_pop(vm->mem, args0); break;
/* inc   */	case 0x7:  ++(*args0); break;
/* dec   */	case 0x8:  --(*args0); break;
/* add   */	case 0x9:  *args0 += *args1; break;
/* sub   */	case 0xA:  *args0 -= *args1; break;
/* mul   */	case 0xB:  *args0 *= *args1; break;
/* div   */	case 0xC:  *args0 /= *args1; break;
/* mod   */	case 0xD:  vm->mem->remainder = *args0 % *args1; break;
/* rem   */	case 0xE:  *args0 = vm->mem->remainder; break;
/* not   */	case 0xF:  *args0 = ~(*args0); break;
/* xor   */	case 0x10:  *args0 ^= *args1;  break;
/* or    */	case 0x11: *args0 |= *args1;   break;
/* and   */	case 0x12: *args0 &= *args1;   break;
/* shl   */	case 0x13: *args0 <<= *args1;  break;
/* shr   */	case 0x14: *args0 >>= *args1;  break;
/* cmp   */	case 0x15: vm->mem->FLAGS =
				((*args0 == *args1) | (*args0 > *args1) << 1);
				break;
/* call	 */	case 0x17: tvm_stack_push(vm->mem, instr_idx);
/* jmp	 */	case 0x16: *instr_idx = *args0 - 1; break;
/* ret   */	case 0x18: tvm_stack_pop(vm->mem, instr_idx);
				break;
/* je    */	case 0x19:
				*instr_idx = (vm->mem->FLAGS & 0x1)
					? *args0 - 1 : *instr_idx;
				break;
/* jne   */	case 0x1A:
				*instr_idx = (!(vm->mem->FLAGS & 0x1))
					? *args0 - 1 : *instr_idx;
				break;
/* jg    */	case 0x1B:
				*instr_idx = (vm->mem->FLAGS & 0x2)
					? *args0 - 1 : *instr_idx;
				break;
/* jge   */	case 0x1C:
				*instr_idx = (vm->mem->FLAGS & 0x3)
					? *args0 - 1 : *instr_idx;
				break;
/* jl    */	case 0x1D:
				*instr_idx = (!(vm->mem->FLAGS & 0x3))
					? *args0 - 1 : *instr_idx;
				break;
/* jle   */	case 0x1E:
				*instr_idx = (!(vm->mem->FLAGS & 0x2))
					? *args0 - 1 : *instr_idx;
				break;
/* prn   */	case 0x1F: printf("%i\n", *args0); break;
/* sete  */	case 0x20:
				*args0 = (vm->mem->FLAGS & 0x1); break;
/* setne */	case 0x21:
				*args0 = (!(vm->mem->FLAGS & 0x1)); break;
/* setg  */	case 0x22:
				*args0 = (vm->mem->FLAGS & 0x2); break;
/* setge */	case 0x23:
				*args0 = (vm->mem->FLAGS & 0x3); break;
/* setl  */	case 0x24:
				*args0 = (!(vm->mem->FLAGS & 0x3)); break;
/* setle */	case 0x25:
				*args0 = (!(vm->mem->FLAGS & 0x2)); break;
/* malloc */	case 0x26:
				vm->mem->registers[0x0].i32 /* eax */ =
            tvm_mem_malloc(vm->mem, 1); break;
	};
}

#endif
