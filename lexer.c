#include "lexer.h"

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
    removeComments("Lexer_Test/t1.txt", "clean1.txt");
    return 0;
}