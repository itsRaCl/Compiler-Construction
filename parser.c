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

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G, vector *input) {
  int n = input->size;
  int stack[200];
  bool stack_terminal[200];

  parseTree *ptree[200];
  int top = -1;
  int treetop = -1;

  stack[++top] = DOLLAR;
  stack_terminal[top] = true;
  stack[++top] = NT_PROGRAM;
  stack_terminal[top] = false;

  ptree[++treetop] = (parseTree *)malloc(sizeof(parseTree));
  if (ptree[treetop] == NULL) {
    printf("Error: Memory allocation failed\n");
    exit(1);
  }
  ptree[treetop]->t.var.nt = NT_PROGRAM;
  ptree[treetop]->t.terminal = false;
  ptree[treetop]->lexeme = NULL;
  ptree[treetop]->line = 0;
  ptree[treetop]->lexemeSize = 0;
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

  int x=5;
  while (i < n && top >= 0) {
    printf("parsing input %d\n", i);

    if (stack[top] == get(input, i)->type && stack_terminal[top] == true) {
      printf("Matched token %d at input %d\n", get(input, i)->type, i);
      if (stack[top] == DOLLAR) {
        printf("Parsing Successful\n");
        return root;
      } else {
        ptree[treetop]->lexeme = get(input, i)->lexeme;
        ptree[treetop]->line = get(input, i)->line;
        ptree[treetop]->lexemeSize = get(input, i)->lexemeSize;
        ptree[treetop]->t.var.t = get(input, i)->type;
        ptree[treetop]->t.terminal = true;
        top--;
        treetop--;
        i++;
        continue;
      }
    }
    printf("Top of stack %d. Terminal : %d\n", stack[top], stack_terminal[top]);
    printf("Current token %d\n", get(input, i)->type);
    grammar_rule rule = G.rules[stack[top]][T.table[stack[top]][get(input, i)->type]];
    if (rule.element_count == 0) {
      printf("Error: Unexpected token %d at line %d\n", get(input, i)->type, get(input, i)->line);
      return NULL;
    }
    // print stack
    printf("Stack: ");
    for (int j = 0; j <= top; j++) {
      printf("%d ", stack[j]);
    }
    printf("\n       ");
    for(int j=0;j<=top;j++){
      printf("%d ",stack_terminal[j]);
    }
    printf("\n");
    top--;
    printf("Popped top of stack\n");
    parseTree *temp = ptree[treetop];
    treetop--;
    for (int j = rule.element_count - 1; j >= 0; j--) {

      // check if epsilon
      if (rule.elements[j].terminal==true && rule.elements[j].var.t == EPSILLON) {
        continue;
      }

      // incrementing top
      top++;
      treetop += 1;
      
      // pushing element on stack
      if(rule.elements[j].terminal){
        stack_terminal[top] = true;
        stack[top] = rule.elements[j].var.t;
      }
      else{
        stack_terminal[top] = false;
        stack[top] = rule.elements[j].var.nt;
      }

      // creating parse tree node
      ptree[treetop] = (parseTree *)malloc(sizeof(parseTree));
      if (ptree[treetop] == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
      }
      if(stack_terminal[top]){
        ptree[treetop]->t.var.t = stack[top];
        ptree[treetop]->t.terminal = true;
      }
      else{
        ptree[treetop]->t.var.nt = stack[top];
        ptree[treetop]->t.terminal = false;
      }
      ptree[treetop]->lexeme = NULL;
      ptree[treetop]->line = -1;
      ptree[treetop]->lexemeSize = 0;
      ptree[treetop]->parent = temp;
      ptree[treetop]->no_of_children = 0;

      // adding child to parent
      temp->children[temp->no_of_children] = ptree[treetop];
      temp->no_of_children++;

      // print stack
      printf("Pushed %d to stack\n", stack[top]);
      printf("Stack: ");
      for (int j = 0; j <= top; j++) {
        printf("%d ", stack[j]);
      }
      printf("\n       ");
      for(int j=0;j<=top;j++){
        printf("%d ",stack_terminal[j]);
      }
      printf("\n");
    }
    x--;
  }
}
void printParseTree(parseTree *PT, FILE *outfile){
      if (PT == NULL) {
        return;
    }
    if (PT->no_of_children != 0 && PT->children[0] != NULL){
        printParseTree(PT->children[0], outfile);
    }
    if (outfile != NULL){
        if (PT != NULL) {
          fprintf(outfile, "%-20s", (PT->t.lexeme != NULL) ? PT->t.lexeme : "----");
          fprintf(outfile, "%-20d", (PT->t.line != -1) ? PT->t.line : -1);
          fprintf(outfile, "%-20d", PT->t.type);

          if ((PT->t.type == TK_RNUM) || (PT->t.type == TK_NUM)) {
              fprintf(outfile, "%-20s", PT->t.lexeme);
          } else {
              fprintf(outfile, "%-20s", "----");
          }
          // if (PT->parent) aayega
          } else {
              fprintf(outfile, "%-20s%-20s%-20s%-20s", "----", "----", "----", "----");
          }
          fprintf(outfile, "%-20s", (PT->no_of_children == 0) ? "YES" : "NO");
          fprintf(outfile, "%-20s", getTokenName(PT->t.type)); 
          fprintf(outfile, "\n");
      }
    for(int i = 1; i < PT->no_of_children; i++){
        if (PT->children[i]!=NULL){
        printParseTree(PT->children[i],outfile);
        }
    }
}
int main() {
  FILE *fp = fopen("Lexer_Test/t5.txt", "r");
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
  // printf("here0");
  fclose(fp);
  // printf("here");
  FILE *outfile = fopen("parse.txt","w");
  if (outfile == NULL) {
    printf("Error: Unable to open file\n");
    return 1;
  }
  if (root == NULL) {
    printf("Error: Parsing failed. Cannot print parse tree.\n");
    fclose(outfile);
    return 1;
}
  printParseTree(root,outfile);
  fclose(outfile);
  return 0;
}
