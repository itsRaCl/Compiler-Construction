#ifndef TRIE
#define TRIE

#include "lexerDef.h"
#define ALPHABET_SIZE 26

typedef struct trie
{
    struct trie* children[ALPHABET_SIZE];
    TOKEN_TYPE tokenType;
} Trie;
typedef Trie* trie;

trie createTrieNode();
void insert(trie root, char* key, TOKEN_TYPE tokenType);
TOKEN_TYPE search(trie root, char* key);

#endif