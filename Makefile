
CC = gcc
CFLAGS = -Wall -ggdb -pedantic 
CFLAGSOPT = -O2
IFLAGS = -I./
LDFLAGS = -L./
LDLIBS = -lqueue

# -lm enlaza la biblioteca matematica, -pthread enlaza la biblioteca de hilos
#
LIBS = -lm -pthread

EJS = p4_e1 p4_e2 p4_e3
########################################################
OBJECTSP4E1 = p4_e1.o bstree.o vertex.o
OBJECTSP4E2 = p4_e2.o graph.o vertex.o stack.o queue.o
OBJECTSP4E3 = p4_e3.o list.o file_utils.o
########################################################

all: $(EJS)

######################################################################
# $@ es el item que aparece a la izquierda de ':'
# $< es el primer item en la lista de dependencias
# $^ son todos los archivos que se encuentran a la derecha de ':'
########################################################################



p4_e1: $(OBJECTSP4E1)
	$(CC) -o $@ $^
p4_e2: $(OBJECTSP4E2)
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS) $(LIBS)
p4_e3: $(OBJECTSP4E3)
	$(CC) -o $@ $^ $(LDFLAGS)

# specify how to compile the .c files
#

graph.o: graph.c graph.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $< 

vertex.o: vertex.c vertex.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $< 

stack.o : stack.c stack.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $< 

queue.o : queue.c queue.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

file_utils.o : file_utils.c file_utils.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

delivery.o : delivery.c delivery.h queue.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

list.o : list.c types.h list.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

bstree.o : bstree.c bstree.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

search_queue.o : search_queue.c search_queue.h types.h
	$(CC) -c $(CFLAGS) $(IFLAGS) -o $@ $<

p4_e1.o: p4_e1.c bstree.h types.h vertex.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
p4_e2.o: p4_e2.c search_queue.h types.h file_utils.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)
p4_e3.o: p4_e3.c list.h types.h file_utils.h
	$(CC) -c -o $@ $< $(CFLAGS) $(IFLAGS)

include Makefile_ext

.PHONY: clean
clean:
	rm  -f *.o $(EJS)
