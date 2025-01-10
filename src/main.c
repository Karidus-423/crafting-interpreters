#include "chunk.h"
#include "debug.h"

int main(int arc, const char *argv[]) {
  byteChunk chunk;
  InitChunk(&chunk);

  int constant = AddConstant(&chunk, 1.2);

  WriteChunk(&chunk, OP_CONSTANT, 123);
  WriteChunk(&chunk, constant, 123);
  WriteChunk(&chunk, OP_RETURN, 123);

  DissasembleChunk(&chunk, "Test Chunk");
  FreeChunk(&chunk);
  return 0;
}
