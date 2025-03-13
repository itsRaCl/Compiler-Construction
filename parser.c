#include "lexer.h"
#include "lexerDef.h"
#include "parserDef.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void createParseTable(FirstFollow F, table *T) {
  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    for (int j = 0; j < TOTAL_TOKENS; j++) {
      T->table[i][j] = -1;
    }
  }
  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    for (int j = 0; j < F.first_count[i]; j++) {
      T->table[i][F.first[i][j]] = F.rule_no[i][j];
    }
    if (F.follow_rule[i] != -1) {
      for (int j = 0; j < F.follow_count[i]; j++) {
        T->table[i][F.follow[i][j]] = F.follow_rule[i];
      }
    }
  }
}

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G,
                                vector *input) {
  int n = input->size;
  int stack[200];

  parseTree *ptree[200];
  int top = -1;
  int treetop = -1;

  stack[++top] = DOLLAR;
  stack[++top] = NT_PROGRAM;

  ptree[++treetop] = (parseTree *)malloc(sizeof(parseTree));
  if (ptree[treetop] == NULL) {
    printf("Error: Memory allocation failed\n");
    exit(1);
  }
  ptree[treetop]->t.type = NT_PROGRAM;
  ptree[treetop]->parent = NULL;
  ptree[treetop]->no_of_children = 0;

  parseTree *root = ptree[treetop];
  int i = 0;
  int erroc = 1, err = 0;

  // print input
  for (int i = 0; i < n; i++) {
    printf("%d ", get(input, i)->type);
  }
  printf("\n");

  printf("Parsing Started\n");

  while (i < n && top > 0) {
    printf("parsing input %d\n", i);

    if (stack[top] == get(input, i)->type) {
      printf("Matched token %d at input %d\n", get(input, i)->type, i);
      if (stack[top] == DOLLAR) {
        printf("Parsing Successful\n");
        return root;
      } else {
        ptree[treetop]->t.lexeme = get(input, i)->lexeme;
        ptree[treetop]->t.line = get(input, i)->line;
        ptree[treetop]->t.lexemeSize = get(input, i)->lexemeSize;
        ptree[treetop]->t.type = get(input, i)->type;
        top--;
        treetop--;
        i++;
        continue;
      }
    }
    printf("Top of stack %d\n", stack[top]);
    printf("Current token %d\n", get(input, i)->type);
    printf("a\n");
    grammar_rule rule =
        G.rules[stack[top]][T.table[stack[top]][get(input, i)->type]];
    printf("b\n");
    if (rule.element_count == 0) {
      printf("Error: Unexpected token %d at line %d\n", get(input, i)->type,
             get(input, i)->line);
      return NULL;
    }
    // print stack
    printf("Stack: ");
    for (int j = 0; j <= top; j++) {
      printf("%d ", stack[j]);
    }
    printf("\n");
    top--;
    printf("Popped top of stack\n");
    parseTree *temp = ptree[treetop];
    treetop--;
    for (int j = rule.element_count - 1; j >= 0; j--) {
      top++;
      treetop += 1;
      stack[top] = rule.elements[j].var.nt;
      ptree[treetop] = (parseTree *)malloc(sizeof(parseTree));
      if (ptree[treetop] == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
      }
      ptree[treetop]->t = *(TOKEN *)malloc(sizeof(TOKEN));
      ptree[treetop]->t.type = stack[top];
      ptree[treetop]->t.lexeme = NULL;
      ptree[treetop]->t.line = -1;
      ptree[treetop]->parent = temp;
      ptree[treetop]->no_of_children = 0;
      temp->children[temp->no_of_children] = ptree[treetop];
      temp->no_of_children++;
      // print stack
      printf("Pushed %d to stack\n", stack[top]);
      printf("Stack: ");
      for (int j = 0; j <= top; j++) {
        printf("%d ", stack[j]);
      }
      printf("\n");
    }
  }
}

int main() {
  FILE *fp = fopen("Lexer_Test/t2.txt", "r");
  if (fp == NULL) {
    printf("Error: Unable to open testcase file\n");
    return 1;
  }
  vector *input = getAllTokens(fp);
  if (input == NULL) {
    printf("Error: Token extraction failed\n");
    fclose(fp);
    return 1;
  }

  // add dollar at the end of input
  tokenInfo dollarToken;
  dollarToken = (tokenInfo)malloc(sizeof(TOKEN));
  dollarToken->type = DOLLAR;
  push(input, dollarToken);

  table T;

  grammar G = initializeGrammar();
  FirstFollow F = computeFirstFollowSet(G);
  createParseTable(F, &T);

  // printf("%d\n",T.table[27][22]);

  // grammar_rule rule = G.rules[27][T.table[27][22]];
  // for(int i=0;i<rule.element_count;i++){
  //   printf("%d ",rule.elements[i].var);
  // }

  parseTree *root = parseInputSourceCode(T, F, G, input);

  if (root == NULL) {
    printf("Error: Parsing failed\n");
    fclose(fp);
    return 1;
  }

  fclose(fp);
  return 0;
}
