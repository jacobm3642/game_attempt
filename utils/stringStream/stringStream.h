#ifndef STRINGSTREAM_H
#define STRINGSTREAM_H

typedef struct stringStream {
  char *string;
  int len;
  int fullness;
}stringStream;

stringStream *initStringStream();

void ssAppend(stringStream *ss, char *string, int len);

void readInFile(stringStream *ss, char *fileName);

#endif
