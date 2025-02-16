#ifndef LEXER_HEADER
#define LEXER_HEADER

#include <stdio.h>
#include "lexerDef.h"



FILE *getStream(FILE *fp);
tokenInfo getNextToken(twinBuffer B);
void removeComments(char *testcaseFile, char *cleanFile);

#endif
