#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

#include "stringStream.h"

stringStream *initStringStream() {
    stringStream *newStringStream = malloc(sizeof(stringStream));
    newStringStream->string = malloc(1);
    newStringStream->len = 1;
    newStringStream->fullness = 0;
    newStringStream->string[0] = '\0';
    return newStringStream;
}

void ssAppend(stringStream *ss, char *string, int len) {
    while ((ss->len - ss->fullness) < len + 1) {
        ss->len *= 2;
        ss->string = realloc(ss->string, ss->len);
    }

    for (int i = 0; i < len; i++) {
        ss->string[ss->fullness + i] = string[i];
    }

    ss->string[ss->fullness + len] = '\0';
    ss->fullness += len;
}

int getFileSize(char *fileName) {
  struct stat st;
  stat(fileName, &st);
  int size = st.st_size;
  return size;
}

void readInFile(stringStream *ss, char *fileName) {
  FILE *fptr;
  fptr = fopen(fileName, "r");
  if (fptr == NULL) {
    return;
  }

  int size = getFileSize(fileName);

  int inChar;
  for (int i; i < size; i++) {
    if ((inChar = fgetc(fptr)) == -1) {
      break;
    } else {
      char c = (char)inChar;
      ssAppend(ss, &c, 1);
    }
  }
  fclose(fptr);
}

