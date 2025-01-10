#ifndef CLOX_DEBUG_H
#define CLOX_DEBUG_H

#include "chunk.h"

void DissasembleChunk(byteChunk *chunk, const char *name);
int DissasembleInstruction(byteChunk *chunk, int offset);

#endif
