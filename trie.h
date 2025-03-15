/*
  Group Number: 43
  NAME                 : ID
  Gyanesh Roy Bhowmik  : 2022A7PS0035P
  Ritvik Singh         : 2022A7PS0045P
  Madhav Gupta         : 2022A7PS0078P
  Clerk Raj Anuj       : 2022A7PS0080P
  Dev Chheda           : 2022A7PS0134P 
*/
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