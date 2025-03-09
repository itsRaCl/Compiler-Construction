#include "vector.h"
#include <stdlib.h>

vector *createVector()
{
    vector *v = (vector *)malloc(sizeof(vector));
    v->size = 0;
    v->capacity = 1;
    v->data = (TOKEN **)malloc(sizeof(TOKEN *) * v->capacity);
    return v;
}

void pushBack(vector *v, TOKEN *t)
{
    if (v->size == v->capacity)
    {
        v->capacity *= 2;
        v->data = (TOKEN **)realloc(v->data, sizeof(TOKEN *) * v->capacity);
    }
    v->data[v->size++] = t;
}

TOKEN *get(vector *v, int index)
{
    if (index < 0 || index >= v->size)
    {
        return NULL;
    }
    return v->data[index];
}

void freeVector(vector *v)
{
    free(v->data);
    free(v);
}
