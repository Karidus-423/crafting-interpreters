#include "chunk.h"
#include "common.h"
#include "debug.h"
#include "virtual-machine.h"
#include <stdio.h>
#include <stdlib.h>

static char *ReadFile(const char *path) {
  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    fprintf(stderr, "Could not open file \"%s\". \n", path);
    exit(74);
  }

  fseek(file, 0L, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    fprintf(stderr, "Not enough memory for read\"%s\". \n", path);
    exit(74);
  }
  size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
  if (bytes_read < file_size) {
    fprintf(stderr, "Could not read file \"%s\". \n", path);
    exit(74);
  }

  buffer[bytes_read] = '\0';
  fclose(file);
  return buffer;
}

void Repl() {
  char line[1024];
  for (;;) {
    printf("> ");

    if (fgets(line, sizeof(line), stdin) != true) {
      printf("\n");
      break;
    }

    Interpret(line);
  }
}

static void RunFile(const char *path) {
  char *source = ReadFile(path);
  interpretResult result = interpret(source);
  free(source);

  if (result == INTERPRET_COMPILE_ERROR)
    exit(65);
  if (result == INTERPRET_RUNTIME_ERROR)
    exit(70);
}

int main(int argc, const char *argv[]) {
  InitVM();

  if (argc == 1) {
    Repl();
  } else if (argc == 2) {
    RunFile(argv[1]);
  } else {
    fprintf(stderr, "Usage: clox [path]\n");
    exit(64);
  }

  FreeVM();
  return 0;
}
