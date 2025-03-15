#ifndef PARSER_HEADER
#define PARSER_HEADER
#include "parserDef.h"
#include <stdio.h>

void createParseTable(FirstFollow F, table *T);

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G,
                                vector *input);

void printParseTree(parseTree *PT, FILE *outfile);
#endif
