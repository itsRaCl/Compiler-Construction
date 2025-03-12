lexer.o: lexer.c lexer.h
	gcc -c lexer.c

trie.o: trie.c trie.h
	gcc -c trie.c

string.o: string.c string.h
	gcc -c string.c

vector.o: vector.c vector.h
	gcc -c vector.c

run_lexer: lexer.o trie.o string.o vector.o
	gcc -o run_lexer lexer.o trie.o string.o vector.o

run: run_lexer
	./run_lexer

clean:
	rm -f *.o run_lexer
