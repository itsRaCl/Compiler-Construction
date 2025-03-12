#ifndef VECTOR
#define VECTOR

#include "lexerDef.h"

typedef struct vector {
    int size;
    int capacity;
    tokenInfo* tokens;
} vector;

typedef vector* Vector;

Vector createVector();
void push(Vector v, tokenInfo token);
tokenInfo pop(Vector v);
tokenInfo get(Vector v, int index);
void freeVector(Vector v);
#endif