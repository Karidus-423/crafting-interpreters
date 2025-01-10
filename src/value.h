#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

#include "common.h"

typedef double Value;

typedef struct {
  int capacity;
  int count;
  Value *values;
} valueArray;

void InitValueArray(valueArray *array);
void WriteValueArray(valueArray *array, Value value);
void FreeValueArray(valueArray *array);
void PrintValue(Value value);

#endif
