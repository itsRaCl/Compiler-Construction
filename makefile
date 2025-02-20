lexer.o: lexer.c lexer.h
	gcc -c lexer.c

trie.o: trie.c trie.h
	gcc -c trie.c

run_lexer: lexer.o trie.o
	gcc -o run_lexer lexer.o trie.o

run: run_lexer
	./run_lexer

clean:
	rm -f *.o run_lexer
