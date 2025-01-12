#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_NEGATE,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_RETURN,
} opCode;

typedef struct {
  int count;
  int capacity;
  int *lines;
  uint8_t *code;
  valueArray constants;
} byteChunk;

void InitChunk(byteChunk *chunk);
void WriteChunk(byteChunk *chunk, uint8_t byte, int line);
void FreeChunk(byteChunk *chunk);
int AddConstant(byteChunk *chunk, Value value);
#endif
