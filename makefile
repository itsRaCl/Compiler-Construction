#   Group Number: 43
#   NAME                 : ID
#   Gyanesh Roy Bhowmik  : 2022A7PS0035P
#   Ritvik Singh         : 2022A7PS0045P
#   Madhav Gupta         : 2022A7PS0078P
#   Clerk Raj Anuj       : 2022A7PS0080P
#   Dev Chheda           : 2022A7PS0134P 

exec: lexer.o trie.o string.o utils.o parser.o driver.o
	gcc -o exec driver.o lexer.o parser.o string.o trie.o utils.o
lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c

trie.o: trie.c trie.h
	gcc -c trie.c

string.o: string.c string.h
	gcc -c string.c

utils.o: utils.c utils.h
	gcc -c utils.c

parser.o: parser.c parserDef.h
	gcc -c parser.c

driver.o: driver.c
	gcc -c driver.c

run_parser: lexer.o trie.o string.o parser.o utils.o
	gcc -o run_parser lexer.o trie.o string.o parser.o utils.o

run: run_parser
	./run_parser

run_lexer: lexer.o trie.o string.o
	gcc -o run_lexer lexer.o trie.o string.o
	./run_lexer

clean:
	rm *.o run_parser run_lexer exec
