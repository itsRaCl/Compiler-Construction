#include "lexer.h"
#include "parser.h"
#include "parserDef.h"
#include "utils.h"
#include <stdlib.h>

char *MAIN_MENU = "What do you want to do:\n\t0. Exit\n\t1. Remove "
                  "Comments\n\t2. Print Lexer Token List\n\t3. Parse Code and "
                  "Print Parse Tree\n\t4. Parse Code and Print Time Taken\n";

int main(int argc, char *argv[]) {
  grammar G = initializeGrammar();
  FirstFollow ff = computeFirstFollowSet(G);
  table T;
  createParseTable(ff, &T);

  if (argc != 3) {
    printf("Invalid No. of Arguments required 2 got %d\n", argc - 1);
    return 1;
  }

  int choice;
  while (1) {
    printf("%s", MAIN_MENU);
    printf("==> ");
    scanf("%d", &choice);

    switch (choice) {
    case 0:
      return 0;
      break;
    case 1:
      printf("==== Comment Free Code ====\n");
      removeComments(argv[1]);
      printf("==== Comment Removal Complete ====\n\n\n");
      break;
    case 2:
      printf("==== Lexer Tokens ====\n");
      FILE *fp = fopen(argv[1], "r");
      Vector tokens = getAllTokens(fp);
      printVector(tokens);
      printf("==== Lexer Tokens Complete ====\n\n\n");
      break;
    case 3:
      printf("==== Parsing... ===");
      FILE *inputFile = fopen(argv[1], "r");
      FILE *outputFile = fopen(argv[2], "r");
      Vector lexerTokens = getAllTokens(inputFile);
      tokenInfo dollarToken;
      dollarToken = (tokenInfo)malloc(sizeof(TOKEN));
      dollarToken->type = DOLLAR;
      push(lexerTokens, dollarToken);
      parseTree *root = parseInputSourceCode(T, ff, G, lexerTokens);

      break;
    case 4:
      printf("4\n");
      break;
    default:
      printf("Invalid Input - Exiting!\n");
      return 0;
    }
  }
}
