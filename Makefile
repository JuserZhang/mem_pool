APP = mem_test
OBJ = test.o mem_pool.o
CC = gcc
CFLAGS = -g -Wall -std=c99

$(APP) : $(OBJ)
	$(CC) $(CFLAGS) -o $(APP) $(OBJ)

mem_pool.o : mem_pool.c mem_pool.h
	$(CC) $(CFLAGS) -c mem_pool.c -o mem_pool.o

test.o : test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

clean :
	rm -f $(APP) $(OBJ)