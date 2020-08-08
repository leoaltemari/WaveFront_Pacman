all:	main.c	grafo.o
	gcc main.c grafo.o -Wall -lm -o exec
grafo.o:	grafo.c
	gcc grafo.c -c
run:
	./exec
clean:
	rm -rf exec *.o
