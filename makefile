lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c

trie.o: trie.c trie.h
	gcc -c trie.c

string.o: string.c string.h
	gcc -c string.c

vector.o: vector.c vector.h
	gcc -c vector.c

utils.o: utils.c utils.h
	gcc -c utils.c

parser.o: parser.c parserDef.h
	gcc -c parser.c

run_parser: lexer.o trie.o string.o vector.o parser.o utils.o
	gcc -o run_parser lexer.o trie.o string.o vector.o parser.o utils.o

run: run_parser
	./run_parser

clean:
	del /Q *.o run_parser.exe
