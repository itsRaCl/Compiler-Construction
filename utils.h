#ifndef UTILS_HEADER
#define UTILS_HEADER
#include "parserDef.h"

grammar initializeGrammar();
FirstFollow computeFirstFollowSet(grammar G);
char *getNonTerminal(NON_TERMINAL nt);

#endif
