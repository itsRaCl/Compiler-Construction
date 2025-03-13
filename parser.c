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

  int x = 5;
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
    grammar_rule rule =
        G.rules[stack[top]][T.table[stack[top]][get(input, i)->type]];
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
    printf("\n       ");
    for (int j = 0; j <= top; j++) {
      printf("%d ", stack_terminal[j]);
    }
    printf("\n");
    top--;
    printf("Popped top of stack\n");
    parseTree *temp = ptree[treetop];
    treetop--;
    for (int j = rule.element_count - 1; j >= 0; j--) {

      // check if epsilon
      if (rule.elements[j].terminal == true &&
          rule.elements[j].var.t == EPSILLON) {
        continue;
      }

      // incrementing top
      top++;
      treetop += 1;

      // pushing element on stack
      if (rule.elements[j].terminal) {
        stack_terminal[top] = true;
        stack[top] = rule.elements[j].var.t;
      } else {
        stack_terminal[top] = false;
        stack[top] = rule.elements[j].var.nt;
      }

      // creating parse tree node
      ptree[treetop] = (parseTree *)malloc(sizeof(parseTree));
      if (ptree[treetop] == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
      }
      if (stack_terminal[top]) {
        ptree[treetop]->t.var.t = stack[top];
        ptree[treetop]->t.terminal = true;
      } else {
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
      for (int j = 0; j <= top; j++) {
        printf("%d ", stack_terminal[j]);
      }
      printf("\n");
    }
    x--;
  }
}
char* getNonTerminal(NON_TERMINAL nt){
  switch (nt) {
    case NT_PROGRAM: return "NT_PROGRAM";
    case NT_MAINFUNCTION: return "NT_MAINFUNCTION";
    case NT_OTHERFUNCTIONS: return "NT_OTHERFUNCTIONS";
    case NT_FUNCTION: return "NT_FUNCTION";
    case NT_INPUT_PAR: return "NT_INPUT_PAR";
    case NT_OUTPUT_PAR: return "NT_OUTPUT_PAR";
    case NT_PARAMETER_LIST: return "NT_PARAMETER_LIST";
    case NT_DATATYPE: return "NT_DATATYPE";
    case NT_PRIMITIVEDATATYPE: return "NT_PRIMITIVEDATATYPE";
    case NT_CONSTRUCTEDDATATYPE: return "NT_CONSTRUCTEDDATATYPE";
    case NT_REMAINING_LIST: return "NT_REMAINING_LIST";
    case NT_STMTS: return "NT_STMTS";
    case NT_TYPEDEFINITIONS: return "NT_TYPEDEFINITIONS";
    case NT_ACTUALORREDEFINED: return "NT_ACTUALORREDEFINED";
    case NT_TYPEDEFINITION: return "NT_TYPEDEFINITION";
    case NT_FIELDDEFINITIONS: return "NT_FIELDDEFINITIONS";
    case NT_FIELDDEFINITION: return "NT_FIELDDEFINITION";
    case NT_FIELDTYPE: return "NT_FIELDTYPE";
    case NT_MOREFIELDS: return "NT_MOREFIELDS";
    case NT_DECLATRATIONS: return "NT_DECLATRATIONS";
    case NT_DECLATRATION: return "NT_DECLATRATION";
    case NT_GLOBAL_OR_NOT: return "NT_GLOBAL_OR_NOT";
    case NT_OTHERSTMTS: return "NT_OTHERSTMTS";
    case NT_STMT: return "NT_STMT";
    case NT_ASSIGNMENTSTMT: return "NT_ASSIGNMENTSTMT";
    case NT_SINGLEORRECID: return "NT_SINGLEORRECID";
    case NT_OPTION_SINGLE_CONSTRUCTED: return "NT_OPTION_SINGLE_CONSTRUCTED";
    case NT_ONEEXPANSION: return "NT_ONEEXPANSION";
    case NT_MOREEXPANSIONS: return "NT_MOREEXPANSIONS";
    case NT_FUNCALLSTMT: return "NT_FUNCALLSTMT";
    case NT_OUTPUTPARAMETERS: return "NT_OUTPUTPARAMETERS";
    case NT_INPUTPARAMETERS: return "NT_INPUTPARAMETERS";
    case NT_ITERATIVESTMT: return "NT_ITERATIVESTMT";
    case NT_CONDITIONALSTMT: return "NT_CONDITIONALSTMT";
    case NT_ELSEPART: return "NT_ELSEPART";
    case NT_IOSTMT: return "NT_IOSTMT";
    case NT_ARITHMETICEXPRESSION: return "NT_ARITHMETICEXPRESSION";
    case NT_EXPPRIME: return "NT_EXPPRIME";
    case NT_TERM: return "NT_TERM";
    case NT_TERMPRIME: return "NT_TERMPRIME";
    case NT_FACTOR: return "NT_FACTOR";
    case NT_HIGHPRECEDENCEOPERATORS: return "NT_HIGHPRECEDENCEOPERATORS";
    case NT_LOWPRECEDENCEOPERATORS: return "NT_LOWPRECEDENCEOPERATORS";
    case NT_BOOLEANEXPRESSION: return "NT_BOOLEANEXPRESSION";
    case NT_VAR: return "NT_VAR";
    case NT_LOGICALOP: return "NT_LOGICALOP";
    case NT_RELATIONALOP: return "NT_RELATIONALOP";
    case NT_RETURNSTMT: return "NT_RETURNSTMT";
    case NT_OPTIONALRETURN: return "NT_OPTIONALRETURN";
    case NT_IDLIST: return "NT_IDLIST";
    case NT_MORE_IDS: return "NT_MORE_IDS";
    case NT_DEFINETYPESTMT: return "NT_DEFINETYPESTMT";
    case NT_A: return "NT_A";
    default: return "INVALID_NON_TERMINAL";
}
}
void printParseTree(parseTree *PT, FILE *outfile) {
  if (PT == NULL) {
    return;
  }
  if (PT->no_of_children != 0 && PT->children[0] != NULL) {
    printParseTree(PT->children[0], outfile);
  }
  if (outfile != NULL) {
    if (PT != NULL) {
      fprintf(outfile, "%-20s", (PT->lexeme != NULL) ? PT->lexeme : "----");
      fprintf(outfile, "%-20d", (PT->line != -1) ? PT->line : -1);
      fprintf(outfile, "%-20s", getTokenName(PT->t.var.t)); // TODO This line needs some fixing
      if ((PT->t.var.t == TK_RNUM) || (PT->t.var.t == TK_NUM)) {
        fprintf(outfile, "%-20s", PT->lexeme);
      } else {
        fprintf(outfile, "%-20s", "----");
      }
      if (PT->parent != NULL){
        fprintf(outfile, "%-20s", getNonTerminal(PT->t.var.nt));
      }
     else {
      fprintf(outfile, "%-20s%-20s%-20s%-20s", "----", "----", "----", "----");
    }
    fprintf(outfile, "%-20s", (PT->no_of_children == 0) ? "YES" : "NO");
    fprintf(outfile, "%-20s", getTokenName(PT->t.var.t));
    fprintf(outfile, "\n");
  }
  for (int i = 1; i < PT->no_of_children; i++) {
    if (PT->children[i] != NULL) {
      printParseTree(PT->children[i], outfile);
    }
  }
}}
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
  FILE *outfile = fopen("parse.txt", "w");
  if (outfile == NULL) {
    printf("Error: Unable to open file\n");
    return 1;
  }
  if (root == NULL) {
    printf("Error: Parsing failed. Cannot print parse tree.\n");
    fclose(outfile);
    return 1;
  }
  printParseTree(root, outfile);
  fclose(outfile);
  return 0;
}
