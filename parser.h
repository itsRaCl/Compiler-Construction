/*
  Group Number: 43
  NAME                 : ID
  Gyanesh Roy Bhowmik  : 2022A7PS0035P
  Ritvik Singh         : 2022A7PS0045P
  Madhav Gupta         : 2022A7PS0078P
  Clerk Raj Anuj       : 2022A7PS0080P
  Dev Chheda           : 2022A7PS0134P 
*/
#ifndef PARSER_HEADER
#define PARSER_HEADER
#include "parserDef.h"
#include <stdio.h>

void createParseTable(FirstFollow F, table *T);

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G, FILE *fp);

void printParseTree(parseTree *PT, FILE *outfile);

FirstFollow computeFirstFollowSet(grammar G);
#endif
