#ifndef VECTOR
#define VECTOR

#include "lexerDef.h"

typedef struct vector {
    int size;
    int capacity;
    TOKEN **data;
} vector;

typedef vector * Vector;
vector *createVector();
void pushBack(vector *v, TOKEN *t);
TOKEN *get(vector *v, int index);
void freeVector(vector *v);
#endif