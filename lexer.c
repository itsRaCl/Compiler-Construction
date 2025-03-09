#include "lexer.h"
#include "trie.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static trie keywordsLookupTable;

STATE_INFO getNextState(STATE currentState, char nextSymbol)
{
    switch (currentState)
    {
        case START:
        {
            if(nextSymbol==';')
            {
                return (STATE_INFO){START, true, TK_SEM, 0};
            }
            else if(nextSymbol==',')
            {
                return (STATE_INFO){START, true, TK_COMMA, 0};
            }
            else if(nextSymbol=='.')
            {
                return (STATE_INFO){START, true, TK_DOT, 0};
            }
            else if(nextSymbol=='(')
            {
                return (STATE_INFO){START, true, TK_OP, 0};
            }
            else if(nextSymbol==')')
            {
                return (STATE_INFO){START, true, TK_CL, 0};
            }
            else if(nextSymbol=='[')
            {
                return (STATE_INFO){START, true, TK_SQL, 0};
            }
            else if(nextSymbol==']')
            {
                return (STATE_INFO){START, true, TK_SQR, 0};
            }
            else if(nextSymbol=='*')
            {
                return (STATE_INFO){START, true, TK_MUL, 0};
            }
            else if(nextSymbol=='/')
            {
                return (STATE_INFO){START, true, TK_DIV, 0};
            }
            else if(nextSymbol=='+')
            {
                return (STATE_INFO){START, true, TK_PLUS, 0};
            }
            else if(nextSymbol=='-')
            {
                return (STATE_INFO){START, true, TK_MINUS, 0};
            }
            else if(nextSymbol=='~')
            {
                return (STATE_INFO){START, true, TK_NOT, 0};
            }
            else if(nextSymbol=='@')
            {
                return (STATE_INFO){S13, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='!')
            {
                return (STATE_INFO){S16, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='&')
            {
                return (STATE_INFO){S18, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='=')
            {
                return (STATE_INFO){S21, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='%')
            {
                return (STATE_INFO){S23, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol==EOF)
            {
                return (STATE_INFO){START, true, EXIT_TOKEN, 0};
            }
            else if(nextSymbol==':')
            {
                return (STATE_INFO){START, true, TK_COLON, 0};
            }
            else if(nextSymbol=='<')
            {
                return (STATE_INFO){S26, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='>')
            {
                return (STATE_INFO){S33, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='_')
            {
                return (STATE_INFO){S37, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='#')
            {
                return (STATE_INFO){S41, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S44, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='b'&&nextSymbol<='d')
            {
                return (STATE_INFO){S57, false, NULL_TOKEN, 0};
            }
            else if((nextSymbol>='a'&&nextSymbol<='z')||(nextSymbol>='A'&&nextSymbol<='Z'))
            {
                return (STATE_INFO){S55, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='\n'||nextSymbol=='\t'||nextSymbol==' ')
            {
                return (STATE_INFO){START, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='\0')
            {
                return (STATE_INFO){START, true, BLANK, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }

        }
        case S13:
        {
            if(nextSymbol=='@')
            return (STATE_INFO){S14, false, NULL_TOKEN, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S14:
        {
            if(nextSymbol=='@')
            return (STATE_INFO){START, true, TK_OR, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S16:
        {
            if(nextSymbol=='=')
            return (STATE_INFO){START, true, TK_NE, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S18:
        {
            if(nextSymbol=='&')
            return (STATE_INFO){S19, false, NULL_TOKEN, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S19:
        {
            if(nextSymbol=='&')
            return (STATE_INFO){START, true, TK_AND, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S21:
        {
            if(nextSymbol=='=')
            return (STATE_INFO){START, true, TK_EQ, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S23:
        {
            if(nextSymbol!='\n')
            return (STATE_INFO){S23, false, NULL_TOKEN, 0};
            else
            return (STATE_INFO){START, true, TK_COMMENT, 0};
        }
        case S26:
        {
            if(nextSymbol=='-')
            return (STATE_INFO){S28, false, NULL_TOKEN, 0};
            else if(nextSymbol=='=')
            return (STATE_INFO){START, true, TK_LE, 0};
            else
            return (STATE_INFO){START, true, TK_LT, 1};
        }
        case S28:
        {
            if(nextSymbol=='-')
            return (STATE_INFO){S29, false, NULL_TOKEN, 0};
            else
            return (STATE_INFO){START, true, TK_LT, 2};
        }
        case S29:
        {
            if(nextSymbol=='-')
            return (STATE_INFO){START, true, TK_ASSIGNOP, 0};
            else
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
        case S33:
        {
            if(nextSymbol=='=')
            return (STATE_INFO){START, true, TK_GE, 0};
            else
            return (STATE_INFO){START, true, TK_GT, 1};
        }
        case S37:
        {
            if((nextSymbol>='a'&&nextSymbol<='z')||(nextSymbol>='A'&&nextSymbol<='Z') ||(nextSymbol>='0'&&nextSymbol<='9'))
            {
                return (STATE_INFO){S38, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S38:
        {
            if((nextSymbol>='a'&&nextSymbol<='z')||(nextSymbol>='A'&&nextSymbol<='Z') ||(nextSymbol>='0'&&nextSymbol<='9'))
            {
                return (STATE_INFO){S38, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S40, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_FUNID, 1};
            }
        }
        case S40:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S40, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_FUNID, 1};
            }
        }
        case S41:
        {
            if(nextSymbol>='a'&&nextSymbol<='z')
            {
                return (STATE_INFO){S42, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S42:
        {
            if(nextSymbol>='a'&&nextSymbol<='z')
            {
                return (STATE_INFO){S42, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_RUID, 1};
            }
        }
        case S44:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S44, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol=='.')
            {
                return (STATE_INFO){S46, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_NUM, 1};
            }
        }
        case S46:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S47, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_NUM, 2};
            }
        }
        case S47:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S48, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S48:
        {
            if(nextSymbol=='E')
            {
                return (STATE_INFO){S50, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_NUM, 2};
            }
        }
        case S50:
        {
            if(nextSymbol=='+'||nextSymbol=='-')
            {
                return (STATE_INFO){S51, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S52, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S51:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){S52, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S52:
        {
            if(nextSymbol>='0'&&nextSymbol<='9')
            {
                return (STATE_INFO){START, true, TK_RNUM, 0};
            }
            else
            {
                return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
            }
        }
        case S55:
        {
            if(nextSymbol>='a'&&nextSymbol<='z')
            {
                return (STATE_INFO){S55, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_FIELDID, 1};
            }
        }
        case S57:
        {
            if(nextSymbol>='a'&&nextSymbol<='z')
            {
                return (STATE_INFO){S55, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='2'&&nextSymbol<='7')
            {
                return (STATE_INFO){S58, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_FIELDID, 1};
            }
        }
        case S58:
        {
            if(nextSymbol>='2'&&nextSymbol<='7')
            {
                return (STATE_INFO){S59, false, NULL_TOKEN, 0};
            }
            else if(nextSymbol>='b'&&nextSymbol<='d')
            {
                return (STATE_INFO){S58, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_ID, 1};
            }
        }
        case S59:
        {
            if(nextSymbol>='2'&&nextSymbol<='7')
            {
                return (STATE_INFO){S59, false, NULL_TOKEN, 0};
            }
            else
            {
                return (STATE_INFO){START, true, TK_ID, 1};
            }
        }
        default:
        {
            return (STATE_INFO){INVALID, false, NULL_TOKEN, 0};
        }
    }
}

void initializeLookupTable()
{
    keywordsLookupTable = createTrieNode();
    insert(keywordsLookupTable, "as", TK_AS);
    insert(keywordsLookupTable, "call", TK_CALL);
    insert(keywordsLookupTable, "definetype", TK_DEFINETYPE);
    insert(keywordsLookupTable, "else", TK_ELSE);
    insert(keywordsLookupTable, "end", TK_END);
    insert(keywordsLookupTable, "endunion", TK_ENDUNION);
    insert(keywordsLookupTable, "endif", TK_ENDIF);
    insert(keywordsLookupTable, "endrecord", TK_ENDRECORD);
    insert(keywordsLookupTable, "endwhile", TK_ENDWHILE);
    insert(keywordsLookupTable, "global", TK_GLOBAL);
    insert(keywordsLookupTable, "if", TK_IF);
    insert(keywordsLookupTable, "input", TK_INPUT);
    insert(keywordsLookupTable, "int", TK_INT);
    insert(keywordsLookupTable, "list", TK_LIST);
    insert(keywordsLookupTable, "output", TK_OUTPUT);
    insert(keywordsLookupTable, "parameter", TK_PARAMETER);
    insert(keywordsLookupTable, "parameters", TK_PARAMETERS);
    insert(keywordsLookupTable, "read", TK_READ);
    insert(keywordsLookupTable, "real", TK_REAL);
    insert(keywordsLookupTable, "record", TK_RECORD);
    insert(keywordsLookupTable, "return", TK_RETURN);
    insert(keywordsLookupTable, "then", TK_THEN);
    insert(keywordsLookupTable, "type", TK_TYPE);
    insert(keywordsLookupTable, "union", TK_UNION);
    insert(keywordsLookupTable, "while", TK_WHILE);
    insert(keywordsLookupTable, "with", TK_WITH);
    insert(keywordsLookupTable, "write", TK_WRITE);
}

bool populate_buffer(twinBuffer B, FILE* fp)
{
    if(B->index>=BUFFER_SIZE)
    {
        for (int i = 0; i < BUFFER_SIZE; i++)
        {
            char ch = fgetc(fp);
            if(ch==EOF)
            {
                // B->buffer[i] = EOF;
                for (int j = i; j < BUFFER_SIZE; j++)
                {
                    B->buffer[j+BUFFER_SIZE] = '\0';
                }
                return false;
            }
            else if(ch=='\n')
            {
                B->buffer[i] = '\n';
                for (int j = i+1; j < BUFFER_SIZE; j++)
                {
                    B->buffer[j+BUFFER_SIZE] = '\0';
                }
                return true;
            }
            else
            {
                B->buffer[i] = ch;
            }
        }
        return false;
    }
    else
    {
        for (int i = BUFFER_SIZE; i < 2*BUFFER_SIZE; i++)
        {
            char ch = fgetc(fp);
            if(ch==EOF)
            {
                // B->buffer[i] = EOF;
                for (int j = i; j < 2*BUFFER_SIZE; j++)
                {
                    B->buffer[j+BUFFER_SIZE] = '\0';
                }
                return false;
            }
            else if(ch=='\n')
            {
                B->buffer[i] = '\n';
                for (int j = i+1; j < 2*BUFFER_SIZE; j++)
                {
                    B->buffer[j+BUFFER_SIZE] = '\0';
                }
                return true;
            }
            else
            {
                B->buffer[i] = ch;
            }
        }
        return true;
    }
}

tokenInfo getNextToken(twinBuffer B)
{
    STATE currentState = START;
    int start = B->index;
    int end = B->index;
    STATE_INFO nextState = getNextState(currentState, B->buffer[start]);
    while (!(nextState.isReturningToken||nextState.nextSTATE==INVALID||nextState.nextSTATE==BLANK))
    {
        printf("Current state: %d\n", nextState.nextSTATE);
        end++;
        end = end%(2*BUFFER_SIZE);
        nextState = getNextState(nextState.nextSTATE, B->buffer[end]);
    }
    if(nextState.nextSTATE==INVALID)
    {
        printf("Error: Invalid token\n");
        B->index = (start+1)%(2*BUFFER_SIZE);
        return NULL;
    }
    else if(nextState.nextSTATE==BLANK)
    {
        B->index = (start+1)%(2*BUFFER_SIZE);
        return NULL;
    }
    else
    {
        printf("Token found\n");
        int redaction = nextState.redaction;
        end = (end-redaction+2*BUFFER_SIZE)%(2*BUFFER_SIZE);
        int size = 0;
        if(end>=start)
        {
            size = end-start+1;
        }
        else
        {
            size = 2*BUFFER_SIZE-start+end+1;
        }
        char* lexeme = (char*)malloc(sizeof(char)*size);
        int i = 0;
        while (start!=end)
        {
            lexeme[i] = B->buffer[start];
            start++;
            start = start%(2*BUFFER_SIZE);
            i++;
        }
        lexeme[i] = B->buffer[start];
        start++;
        start = start%(2*BUFFER_SIZE);
        tokenInfo token = (tokenInfo)malloc(sizeof(TOKEN));
        token->lexeme = lexeme;
        token->lexemeSize = size;
        token->line = B->line;
        if(nextState.tokenType==TK_FIELDID)
        {
            token->type = search(keywordsLookupTable, lexeme);
        }
        else if(nextState.tokenType==TK_FUNID)
        {
            if(strcpy(lexeme, "main")==0)
            {
                token->type = TK_MAIN;
            }
            else
            {
                token->type = TK_FUNID;
            }
        }
        else
        {
            token->type = nextState.tokenType;
        }
        B->index = start;
        return token;
    }
}

void printbuffer(twinBuffer B)
{
    for (int i = 0; i < 2*BUFFER_SIZE; i++)
    {
        printf("%d ", B->buffer[i]);
    }
    printf("\n");
}

FILE* getStream(FILE* fp)
{
    FILE* stream = fopen("tokens.txt", "w");
    twinBuffer B = (twinBuffer)malloc(sizeof(TWIN_BUFFER));
    B->index = 2*BUFFER_SIZE-1;
    B->line = 1;
    populate_buffer(B, fp);
    B->index = 0;
    populate_buffer(B, fp);
    initializeLookupTable();
    fprintf(stream, "lexeme token\n");
    // while (B->buffer[B->index]!=EOF)
    // {
        int before = B->index;
        tokenInfo token = getNextToken(B);
        printbuffer(B);
        if(token!=NULL)
        {
            fprintf(stream, "%s %d\n", token->lexeme, token->type);
        }
        if(B->buffer[B->index]=='\n'||token->type==TK_COMMENT)
        {
            B->line++;
        }
        int after = B->index;
        if((before<BUFFER_SIZE && after>=BUFFER_SIZE)||(before>=BUFFER_SIZE && after<BUFFER_SIZE))
        {
            populate_buffer(B, fp);
        }
    // } 
    fclose(stream);
    return stream;
}

void removeComments(char *testcaseFile, char *cleanFile)
{
    FILE* testcaseFPTR = fopen(testcaseFile, "r");

    if(testcaseFPTR == NULL)
    {
        printf("Error: Unable to open testcase file %s\n", testcaseFile);
        return;
    }

    FILE* cleanflieFPTR = fopen(cleanFile, "w");

    if(cleanflieFPTR == NULL)
    {
        printf("Error: Unable to open clean file %s\n", cleanFile);
        return;
    }

    char ch = fgetc(testcaseFPTR);
    while (ch!=EOF)
    {
        if(ch=='%')
        {
            while (ch!='\n'&&ch!=EOF)
            {
                ch = fgetc(testcaseFPTR);
            }
            if (ch=='\n')
            {
                ch = fgetc(testcaseFPTR);
            }
            
        }
        else
        {
            fputc(ch, cleanflieFPTR);
            ch = fgetc(testcaseFPTR);
        }
    }
    fclose(testcaseFPTR);
    fclose(cleanflieFPTR);
}

int main()
{
    FILE* fp = fopen("Lexer_Test/t3.txt", "r");
    if(fp==NULL)
    {
        printf("Error: Unable to open testcase file\n");
        return 1;
    }
    FILE* stream = getStream(fp);
    fclose(fp);
    fclose(stream);
    return 0;
}