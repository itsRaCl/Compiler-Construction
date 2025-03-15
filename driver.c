/*
  Group Number: 43
  NAME                 : ID
  Gyanesh Roy Bhowmik  : 2022A7PS0035P
  Ritvik Singh         : 2022A7PS0045P
  Madhav Gupta         : 2022A7PS0078P
  Clerk Raj Anuj       : 2022A7PS0080P
  Dev Chheda           : 2022A7PS0134P
*/
#include "lexer.h"
#include "parser.h"
#include "parserDef.h"
#include "utils.h"
#include <time.h>

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
      getStream(fp);
      printf("==== Lexer Tokens Complete ====\n\n\n");
      break;
    case 3:
      printf("==== Parsing... ====\n");
      FILE *inputFile = fopen(argv[1], "r");
      FILE *outputFile = fopen(argv[2], "w");
      if (inputFile == NULL) {
        printf("Invalid Input File!\n");
        return 1;
      }

      parseTree *root = parseInputSourceCode(T, ff, G, inputFile);
      printParseTree(root, outputFile);
      printf("Source Code Parsed and parse tree printed into %s\n", argv[2]);
      fclose(inputFile);
      fclose(outputFile);
      break;
    case 4:
      printf("==== Parsing... ====\n");
      FILE *inptFile = fopen(argv[1], "r");
      if (inptFile == NULL) {
        printf("Invalid Input File!\n");
        return 1;
      }
      clock_t start_time, end_time;
      start_time = clock();
      parseTree *pt = parseInputSourceCode(T, ff, G, inptFile);
      end_time = clock();
      printf("Source Code Parsed");
      printf("Time Taken to Parse: %ld\n", end_time - start_time);
      printf("Time Taken to Parse (seconds): %lf\n",
             (float)(end_time - start_time) / CLOCKS_PER_SEC);
      fclose(inptFile);
      break;
    default:
      printf("Invalid Input - Exiting!\n");
      return 0;
    }
  }
}
