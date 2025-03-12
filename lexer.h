#ifndef LEXER_HEADER
#define LEXER_HEADER

#include <stdio.h>
#include "lexerDef.h"



FILE *getStream(FILE *fp);
tokenInfo getNextToken(twinBuffer B, FILE *fp);
void removeComments(char *testcaseFile, char *cleanFile);

#endif
