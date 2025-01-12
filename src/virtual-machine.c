#include "virtual-machine.h"
#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "value.h"
#include <stdio.h>

VM virtual_machine;

static void ResetStack() { virtual_machine.stack_top = virtual_machine.stack; }

void Push(Value value) {
  *virtual_machine.stack_top = value;
  virtual_machine.stack_top++;
}

Value Pop() {
  virtual_machine.stack_top--;
  return *virtual_machine.stack_top;
}

void InitVM() { ResetStack(); }

void FreeVM() {}

static interpretResult Run() {
#define READ_BYTE() (*virtual_machine.ip++)
#define READ_CONSTANT() (virtual_machine.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op)                                                          \
  do {                                                                         \
    double b = Pop();                                                          \
    double a = Pop();                                                          \
    Push(a op b);                                                              \
  } while (false)

  for (;;) {

#ifdef DEBUG_TRACE_EXECUTION
    printf("		");
    for (Value *slot = virtual_machine.stack; slot < virtual_machine.stack_top;
         slot++) {
      printf("[");
      PrintValue(*slot);
      printf("]");
    }
    printf("\n");
    DissasembleInstruction(
        virtual_machine.chunk,
        (int)(virtual_machine.ip - virtual_machine.chunk->code));
#endif

    uint8_t instruction;
    switch (instruction = READ_BYTE()) {

    case OP_CONSTANT: {
      Value constant = READ_CONSTANT();
      Push(constant);
      PrintValue(constant);
      printf("\n");
      break;
    }
    case OP_ADD: {
      BINARY_OP(+);
      break;
    }
    case OP_SUBTRACT: {
      BINARY_OP(-);
      break;
    }
    case OP_MULTIPLY: {
      BINARY_OP(*);
      break;
    }
    case OP_DIVIDE: {
      BINARY_OP(/);
      break;
    }
    case OP_NEGATE: {
      Value *to_negate = virtual_machine.stack_top - 1;
      *to_negate = *to_negate * -1;
      /*Push(-Pop());*/
      break;
    }
    case OP_RETURN: {
      PrintValue(Pop());
      printf("\n");
      return INTERPRET_OK;
    }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

interpretResult Interpret(byteChunk *chunk) {
  virtual_machine.chunk = chunk;
  virtual_machine.ip = virtual_machine.chunk->code;
  return Run();
}
