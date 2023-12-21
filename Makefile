all: master worker

master: socket
	gcc src/master.c -o bin/master bin/socket.o

worker: socket
	gcc src/worker.c -o bin/worker bin/socket.o

socket:
	gcc -c lib/socket.c -o bin/socket.o

clean:
	rm -f bin/*