#include "vector.h"
#include <stdlib.h>
#include <stdio.h>

Vector createVector()
{
    Vector v = (Vector)malloc(sizeof(vector));
    v->size = 0;
    v->capacity = 1;
    v->tokens = (tokenInfo*)malloc(sizeof(tokenInfo)*v->capacity);
    return v;
}

void push(Vector v, tokenInfo token)
{
    if(v->size==v->capacity)
    {
        v->capacity = 2*v->capacity;
        v->tokens = (tokenInfo*)realloc(v->tokens, sizeof(tokenInfo)*v->capacity);
    }
    v->tokens[v->size] = token;
    v->size++;
}

tokenInfo pop(Vector v)
{
    if(v->size==0)
    {
        return NULL;
    }
    v->size--;
    return v->tokens[v->size];
}

tokenInfo get(Vector v, int index)
{
    if(index>=v->size)
    {
        return NULL;
    }
    return v->tokens[index];
}

void freeVector(Vector v)
{
    for (int i = 0; i < v->size; i++)
    {
        free(v->tokens[i]->lexeme);
        free(v->tokens[i]);
    }
    free(v->tokens);
    free(v);
}

