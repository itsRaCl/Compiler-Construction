#ifndef LEXER_HEADER
#define LEXER_HEADER

#include "lexerDef.h"
#include <stdio.h>

void getStream(FILE *fp);
tokenInfo getNextToken(twinBuffer B, FILE *fp);
void removeComments(char *testcaseFile);
tokenInfo nextToken(twinBuffer B, FILE *fp);
char *getTokenName(TOKEN_TYPE type);
void populate_buffer(twinBuffer B, FILE *fp);
void initializeLookupTable();
bool handle_valid_error(tokenInfo token);

#endif
