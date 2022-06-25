CC = gcc
CFLAGS = -Wall -Iinclude

client: *.c
	$(CC) $(CFLAGS) -o client *.c

run: client
	./client

valgrind: client
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --show-leak-kinds=all --num-callers=20 --track-fds=yes ./client

clean:
	rm -f *.o client
