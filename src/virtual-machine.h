#ifndef CLOX_VIRTUAL_MACHINE_H
#define CLOX_VIRTUAL_MACHINE_H
#include "chunk.h"
#include <stdint.h>

#define STACK_MAX 256

typedef struct {
  byteChunk *chunk;
  uint8_t *ip;
  Value stack[STACK_MAX];
  Value *stack_top;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR,
} interpretResult;

void InitVM();
void FreeVM();

interpretResult Interpret(byteChunk *chunk);

void Push(Value value);
Value Pop();

#endif
