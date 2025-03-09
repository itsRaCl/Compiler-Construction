#include "lexerDef.h"
#include "parserDef.h"

void createParseTable(FirstFollow F, table T) {
    for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
        for (int j = 0; j < TOTAL_TOKENS; j++) {
            T.table[i][j] = -1;
        }
    }
    for (int i = 0; i < NON_TERMINAL_COUNT; i++) {
        for (int j = 0; j < F.first_count[i]; j++) {
            T.table[i][F.first[i][j]] = F.rule_no[i][j];
        }
        if (F.follow_rule[i] != -1) {
            for (int j = 0; j < F.follow_count[i]; j++) {
                T.table[i][F.follow[i][j]] = F.follow_rule[i];
            }
        }
    }
}