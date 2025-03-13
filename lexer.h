#ifndef LEXER_HEADER
#define LEXER_HEADER

#include <stdio.h>
#include "lexerDef.h"
#include "vector.h"



FILE *getStream(FILE *fp);
tokenInfo getNextToken(twinBuffer B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);
vector* getAllTokens(FILE* fp);
char* getTokenName(TOKEN_TYPE type); 


#endif
