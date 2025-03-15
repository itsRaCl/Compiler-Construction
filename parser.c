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

FirstFollow computeFirstFollowSet(grammar G) {
  FirstFollow ff;

  bool firstComputed[NON_TERMINAL_COUNT];
  NON_TERMINAL **followDep;
  followDep =
      (NON_TERMINAL **)malloc(sizeof(NON_TERMINAL *) * NON_TERMINAL_COUNT);

  int depCount[NON_TERMINAL_COUNT];

  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    ff.first_count[i] = 0;
    ff.follow_count[i] = 0;
    ff.first_has_epsillon[i] = false;
    ff.follow_rule[i] = -1;
    firstComputed[i] = false;
    depCount[i] = 0;
    followDep[i] = (NON_TERMINAL *)calloc(MAX_RULE_SIZE, sizeof(NON_TERMINAL));
  }

  // Computing First Set for all Non Terminals
  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    computeFirstRec(&ff, (NON_TERMINAL)i, G, firstComputed);
  }

  // Base Follow set for start symbol '<program>'
  ff.follow[NT_PROGRAM][0] = DOLLAR;
  ff.follow_count[NT_PROGRAM] = 1;

  // Computing Follow Set for all Non Terminals
  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    for (int j = 0; j < G.rule_count[i]; j++) {
      followHelper(&ff, G.rules[i][j], (NON_TERMINAL)i, followDep, depCount);
    }
  }

  // Clearing follow set dependency between rules
  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    if (depCount[i] > 0) {
      clearDependency(i, followDep, depCount, &ff);
    }
  }

  for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
    free(followDep[i]);
  }
  free(followDep);

  return ff;
}

parseTree *parseInputSourceCode(table T, FirstFollow F, grammar G, FILE *fp) {
  // declaring symbol stack and tree node stack
  grammar_element *symbolStack[200];
  parseTree *treeNodeStack[200];

  // initializing twinBuffer
  twinBuffer B = (twinBuffer)malloc(sizeof(TWIN_BUFFER));
  for (int i = 0; i < 2 * BUFFER_SIZE; i++) {
    B->buffer[i] = '\0';
  }
  B->index = 2 * BUFFER_SIZE - 1;
  B->line = 1;
  populate_buffer(B, fp);
  B->index = 0;
  populate_buffer(B, fp);

  // initializing LookUp Table
  initializeLookupTable();

  int symbolStackTop = 0;
  int treeNodeStackTop = 0;
  int lookAheadPointer = 0;
  int oldLineNo = -1;
  bool error_encountered = false;

  parseTree *root = malloc(sizeof(parseTree));
  root->ele.symbol.terminal = false;
  root->ele.symbol.var.nt = NT_PROGRAM;
  root->ele.line = -1;
  root->parent = NULL;

  treeNodeStack[treeNodeStackTop] = root;

  // Constructing dollar and program symbol for stack
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
  tokenInfo a = nextToken(B, fp);
  while (B->buffer[B->index] != '\0' && symbolStackTop >= 0) {
    grammar_element *X = symbolStack[symbolStackTop];

    // if top of the stack is a terminal
    if (X->terminal) {
      if (X->var.t == DOLLAR && a->type == DOLLAR) {
        // if top of stack is dollar and input token is dollar
        // parsing is complete
        break;
      } else if (X->var.t == a->type) {
        // top of stack terminal matches with input token
        // construct and push node to the treeNodeStack
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
        free(a);
        a = nextToken(B, fp);
      } else {
        // top of stack terminal does not match with input token
        error_encountered = true;
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          free(a);
          a = nextToken(B, fp);
          continue;
        } else {
          oldLineNo = a->line;
          printf(
              "Line %02d: Syntax Error : The token %s for lexeme %s does not "
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
      // if top of stack is a non terminal
      NON_TERMINAL nt = X->var.nt;
      int rule_no = T.table[nt][a->type];
      if (rule_no == -1) {
        // if entry of parse table is -1 (ERROR)
        error_encountered = true;
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          free(a);
          a = nextToken(B, fp);
          continue;
        } else {
          oldLineNo = a->line;
          printf("Line %02d: Syntax Error : Invalid token %s encountered with "
                 "value %s stack top %s\n",
                 a->line, getTokenName(a->type), a->lexeme, getNonTerminal(nt));
          lookAheadPointer++;
          free(a);
          a = nextToken(B, fp);
        }
      } else if (rule_no == -2) {
        // if entry of parse table is -2 (SYN)
        error_encountered = true;
        if (oldLineNo == a->line) {
          lookAheadPointer++;
          free(a);
          a = nextToken(B, fp);
          continue;
        } else {
          oldLineNo = a->line;
          printf("Line %02d: Syntax Error : Invalid token %s encountered with "
                 "value %s stack top %s\n",
                 a->line, getTokenName(a->type), a->lexeme, getNonTerminal(nt));
          free(X);
          symbolStack[symbolStackTop] = NULL;
          symbolStackTop--;
          treeNodeStackTop--;
        }
      } else {
        // if entry of parse table is a valid rule
        // remove non terminal from stack
        grammar_rule rule = G.rules[nt][rule_no];
        parseTree *node = treeNodeStack[treeNodeStackTop];
        treeNodeStackTop--;
        free(X);
        symbolStack[symbolStackTop] = NULL;
        symbolStackTop--;

        if (G.has_epsillon[nt] && rule_no == G.rule_count[nt]) {
          // if the rule is an epsillon rule
          // make epsillon node and add to children
          node->no_of_children = 1;
          parseTree *childNode = (parseTree *)malloc(sizeof(parseTree));
          childNode->ele.symbol.terminal = true;
          childNode->ele.symbol.var.t = EPSILLON;
          childNode->ele.line = -1;
          childNode->parent = node;
          childNode->no_of_children = 0;
          node->children[0] = childNode;
        } else {
          // if the rule is not a non epsillon rule
          // make child nodes for each element in the rule

          node->no_of_children = rule.element_count;

          // push child nodes on the treeNodeStack in reverse order
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
  // check if the input has been completely parsed
  // and symbol stack is DOLLAR
  if (!(symbolStackTop == 0 && symbolStack[symbolStackTop]->var.t == DOLLAR)) {
    error_encountered = true;
    printf("Syntax Error : Tokens parsed but stack not empty\n");
  } else if (a->type != DOLLAR) {
    error_encountered = true;
    printf("Syntax Error : Stack empty but tokens not parsed\n");
  }
  // freeing memory used by twin buffer
  free(B);

  if (!error_encountered)
    printf("COMPILATION SUCCESS!\n");
  else
    printf("COMPILATION FAILED\n");

  // clearing symbol stack (memory cleanup)
  while (symbolStackTop >= 0) {
    if (symbolStack[symbolStackTop] != NULL) {
      free(symbolStack[symbolStackTop]);
    }
    symbolStackTop--;
  }

  return root;
}

void printParseTree(parseTree *PT, FILE *outfile) {
  if (PT == NULL) {
    return;
  }
  static int flag = 1;
  if (flag) {
    flag = 0;
    // print this -> "lexeme CurrentNodeLineno tokenName valueIfNumber parentNodeSymbol isLeafNode(yes/no) NodeSymbol"
    fprintf(outfile, "%-30s%-30s%-30s%-30s%-30s%-30s%-30s\n\n", "lexeme",
            "lineno", "token", "valueIfNumber", "parentNodeSymbol",
            "isLeafNode(yes/no)", "NodeSymbol");
  }
  if (PT->no_of_children != 0 && PT->children[0] != NULL) {
    printParseTree(PT->children[0], outfile);
  }
  if (outfile != NULL) {
    if (PT != NULL) {
      fprintf(outfile, "%-30s",
              (PT->ele.lexeme != NULL) ? PT->ele.lexeme : "----");
      fprintf(outfile, "%-30d", PT->ele.line);

      if (PT->ele.symbol.terminal) {
        fprintf(outfile, "%-30s", getTokenName(PT->ele.symbol.var.t));
      } else {
        fprintf(outfile, "%-30s", getNonTerminal(PT->ele.symbol.var.nt));
      }
      if ((PT->ele.symbol.terminal) && ((PT->ele.symbol.var.t == TK_RNUM) ||
                                        (PT->ele.symbol.var.t == TK_NUM))) {
        fprintf(outfile, "%-30s", PT->ele.lexeme);
      } else {
        fprintf(outfile, "%-30s", "----");
      }
      if (PT->parent != NULL) {
        fprintf(outfile, "%-30s",
                getNonTerminal(PT->parent->ele.symbol.var.nt));
        fprintf(outfile, "%-30s", (PT->no_of_children == 0) ? "YES" : "NO");
        fprintf(outfile, "%-30s", getTokenName(PT->ele.symbol.var.t));
        fprintf(outfile, "\n");
      } else {
        fprintf(outfile, "%-30s%-30s%-30s\n", "----", "----", "----");
      }
    }
    for (int i = 1; i < PT->no_of_children; i++) {
      if (PT->children[i] != NULL) {
        printParseTree(PT->children[i], outfile);
      }
    }
  }
}
