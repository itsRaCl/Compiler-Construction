/*
  Group Number: 43
  NAME                 : ID
  Gyanesh Roy Bhowmik  : 2022A7PS0035P
  Ritvik Singh         : 2022A7PS0045P
  Madhav Gupta         : 2022A7PS0078P
  Clerk Raj Anuj       : 2022A7PS0080P
  Dev Chheda           : 2022A7PS0134P 
*/
#include "trie.h"
#include <stdlib.h>

trie createTrieNode()
{
    trie node = (trie)malloc(sizeof(Trie));
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        node->children[i] = NULL;
    }
    node->tokenType = TK_FIELDID;
    return node;
}

void insert(trie root, char* key, TOKEN_TYPE tokenType)
{
    trie temp = root;
    for (int i = 0; key[i] != '\0'; i++)
    {
        int index = key[i] - 'a';
        if (temp->children[index] == NULL)
        {
            temp->children[index] = createTrieNode();
        }
        temp = temp->children[index];
    }
    temp->tokenType = tokenType;
}

TOKEN_TYPE search(trie root, char* key)
{
    trie temp = root;
    for (int i = 0; key[i] != '\0'; i++)
    {
        int index = key[i] - 'a';
        if (temp->children[index] == NULL)
        {
            return TK_FIELDID;
        }
        temp = temp->children[index];
    }
    return temp->tokenType;
}