all:

socket:
	gcc -c lib/socket.c -o bin/socket.o

clean:
	rm -f bin/*