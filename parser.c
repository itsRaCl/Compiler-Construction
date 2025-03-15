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
    } else {
      for (int j = 0; j < F.follow_count[i]; j++) {
        if (T->table[i][F.follow[i][j]] == -1) {
          T->table[i][F.follow[i][j]] = -2;
        }
      }
    }
  }
}

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G,
                                vector *input) {
  grammar_element *symbolStack[200];
  parseTree *treeNodeStack[200];

  int symbolStackTop = 0;
  int treeNodeStackTop = 0;
  int lookAheadPointer = 0;
  int oldLineNo = -1;

  parseTree *root = malloc(sizeof(parseTree));
  root->ele.symbol.terminal = false;
  root->ele.symbol.var.nt = NT_PROGRAM;
  root->ele.line = -1;
  root->parent = NULL;

  treeNodeStack[treeNodeStackTop] = root;

  grammar_element *dollar = (grammar_element *)malloc(sizeof(grammar_element));
  dollar->terminal = true;
  dollar->var.t = DOLLAR;
  grammar_element *program = (grammar_element *)malloc(sizeof(grammar_element));
  program->terminal = false;
  program->var.nt = NT_PROGRAM;

  // Inserting dollar as bottom symbol of stack
  symbolStack[symbolStackTop] = dollar;

  // Inserting start symbol to stack <program>
  symbolStackTop++;
  symbolStack[symbolStackTop] = program;

  // Following Naming in Slides
  while (lookAheadPointer < input->size && symbolStackTop >= 0) {
    // printf("%d\n", lookAheadPointer);
    grammar_element *X = symbolStack[symbolStackTop];
    TOKEN *a = get(input, lookAheadPointer);

    if (X->terminal) {
      if (X->var.t == DOLLAR && a->type == DOLLAR) {
        break;
      } else if (X->var.t == a->type) {
        parseTree *node = treeNodeStack[treeNodeStackTop];
        node->ele.symbol.terminal = true;
        node->ele.symbol.var.t = a->type;
        node->ele.line = a->line;
        node->ele.lexeme = a->lexeme;
        node->ele.lexemeSize = a->lexemeSize;

        free(X);
        symbolStack[symbolStackTop] = NULL;
        symbolStackTop--;
        treeNodeStackTop--;
        lookAheadPointer++;
      } else {
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          continue;
        } else {
          oldLineNo = a->line;
          printf("Line %d: Syntax Error. The token %s for lexeme %s does not "
                 "match with the expected token %s\n",
                 a->line, getTokenName(a->type), a->lexeme,
                 getTokenName(X->var.t));
          free(X);
          symbolStack[symbolStackTop] = NULL;
          symbolStackTop--;
          treeNodeStackTop--;
        }
      }
    } else {
      NON_TERMINAL nt = X->var.nt;
      int rule_no = T.table[nt][a->type];
      if (rule_no == -1) {
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          continue;
        } else {
          oldLineNo = a->line;
          printf("Line %d: Syntax Error. Invalid token %s encountered with "
                 "value %s stack top %s\n",
                 a->line, getTokenName(a->type), a->lexeme, getNonTerminal(nt));
          lookAheadPointer++;
        }
      } else if (rule_no == -2) {
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          continue;
        } else {
          oldLineNo = a->line;
          printf("Line %d: Syntax Error. Invalid token %s encountered with "
                 "value %s stack top %s\n",
                 a->line, getTokenName(a->type), a->lexeme, getNonTerminal(nt));
          free(X);
          symbolStack[symbolStackTop] = NULL;
          symbolStackTop--;
          treeNodeStackTop--;
        }
      } else {
        grammar_rule rule = G.rules[nt][rule_no];
        parseTree *node = treeNodeStack[treeNodeStackTop];
        treeNodeStackTop--;
        free(X);
        symbolStack[symbolStackTop] = NULL;
        symbolStackTop--;

        if (G.has_epsillon[nt] && rule_no == G.rule_count[nt]) {
          node->no_of_children = 1;
          parseTree *childNode = (parseTree *)malloc(sizeof(parseTree));
          childNode->ele.symbol.terminal = true;
          childNode->ele.symbol.var.t = EPSILLON;
          childNode->ele.line = -1;
          childNode->parent = node;
          childNode->no_of_children = 0;
          node->children[0] = childNode;
        } else {

          node->no_of_children = rule.element_count;

          for (int i = rule.element_count - 1; i >= 0; i--) {
            parseTree *childNode = (parseTree *)malloc(sizeof(parseTree));

            childNode->ele.symbol.terminal = rule.elements[i].terminal;
            childNode->ele.line = -1;

            if (rule.elements[i].terminal) {
              childNode->ele.symbol.var.t = rule.elements[i].var.t;
            } else {
              childNode->ele.symbol.var.nt = rule.elements[i].var.nt;
            }

            childNode->parent = node;
            childNode->no_of_children = 0;
            node->children[i] = childNode;

            treeNodeStack[++treeNodeStackTop] = childNode;
            grammar_element *ele =
                (grammar_element *)malloc(sizeof(grammar_element));
            ele->terminal = rule.elements[i].terminal;
            if (ele->terminal) {
              ele->var.t = rule.elements[i].var.t;
            } else {
              ele->var.nt = rule.elements[i].var.nt;
            }

            symbolStack[++symbolStackTop] = ele;
          }
        }
      }
    }
  }

  return root;
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
      fprintf(outfile, "%-25s",
              (PT->ele.lexeme != NULL) ? PT->ele.lexeme : "----");
      fprintf(outfile, "%-25d", PT->ele.line);

      if (PT->ele.symbol.terminal) {
        fprintf(outfile, "%-25s", getTokenName(PT->ele.symbol.var.t));
      } else {
        fprintf(outfile, "%-25s", getNonTerminal(PT->ele.symbol.var.nt));
      }
      if ((PT->ele.symbol.var.t == TK_RNUM) ||
          (PT->ele.symbol.var.t == TK_NUM)) {
        fprintf(outfile, "%-25s", PT->ele.lexeme);
      } else {
        fprintf(outfile, "%-25s", "----");
      }
      if (PT->parent != NULL) {
        fprintf(outfile, "%-25s",
                getNonTerminal(PT->parent->ele.symbol.var.nt));
        fprintf(outfile, "%-25s", (PT->no_of_children == 0) ? "YES" : "NO");
        fprintf(outfile, "%-25s", getTokenName(PT->ele.symbol.var.t));
        fprintf(outfile, "\n");
      } else {
        fprintf(outfile, "%-25s%-25s%-25s\n", "----", "----", "----");
      }
    }
    for (int i = 1; i < PT->no_of_children; i++) {
      if (PT->children[i] != NULL) {
        printParseTree(PT->children[i], outfile);
      }
    }
  }
}

int main() {
  FILE *fp = fopen("Lexer_Test/t6.txt", "r");
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
