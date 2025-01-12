#include "chunk.h"
#include "memory.h"
#include "value.h"
#include <stdint.h>
#include <stdlib.h>

void InitChunk(byteChunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  InitValueArray(&chunk->constants);
}

void WriteChunk(byteChunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) { //+1 prevents going beyond array.
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);

    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->lines[chunk->count] = line;
  chunk->count++;
}

void FreeChunk(byteChunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  FreeValueArray(&chunk->constants);
  InitChunk(chunk);
}

int AddConstant(byteChunk *chunk, Value value) {
  WriteValueArray(&chunk->constants, value);
  return chunk->constants.count; // Removed -1 as it was not working right.
}
