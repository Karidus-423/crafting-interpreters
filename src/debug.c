#include "debug.h"
#include "chunk.h"
#include "value.h"
#include <stdio.h>

void DissasembleChunk(byteChunk *chunk, const char *name) {
  printf("====%s====\n", name);
  for (int offset = 0; offset < chunk->count;) {
    offset = DissasembleInstruction(chunk, offset);
  }
}

static int SimpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int ConstantInstruction(const char *name, byteChunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];

  printf("%-16s %4d '", name, constant);
  PrintValue(chunk->constants.values[constant]);
  printf("'\n");

  return offset + 2;
}

int DissasembleInstruction(byteChunk *chunk, int offset) {
  printf("%04d", offset);

  if (offset > 0 && chunk->lines[offset] == chunk->lines[offset - 1]) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->lines[offset]);
  }

  uint8_t instruction = chunk->code[offset];

  switch (instruction) {
  case OP_CONSTANT:
    return ConstantInstruction("OP_CONSTANT", chunk, offset);
  case OP_RETURN:
    return SimpleInstruction("OP_RETURN", offset);
  default:
    printf("Unkown OpCode %d\n", instruction);
    return offset + 1;
  }
}
