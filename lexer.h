/*
  Group Number: 43
  NAME                 : ID
  Gyanesh Roy Bhowmik  : 2022A7PS0035P
  Ritvik Singh         : 2022A7PS0045P
  Madhav Gupta         : 2022A7PS0078P
  Clerk Raj Anuj       : 2022A7PS0080P
  Dev Chheda           : 2022A7PS0134P 
*/
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
