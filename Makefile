all: client server

client: src/client.c master worker pipe queue socket socket_server
	gcc src/client.c -o bin/client bin/queue.o bin/pipe.o bin/socket.o bin/socket_server.o bin/worker.o

server: src/server.c worker socket
	gcc src/server.c -o bin/server bin/socket.o bin/worker.o

master: src/master.c
	gcc -c src/master.c -o bin/master.o

worker: src/worker.c
	gcc -c src/worker.c -o bin/worker.o

buffer: src/buffer.c
	gcc -c src/buffer.c -o bin/buffer.o

pipe: src/pipe.c
	gcc -c src/pipe.c -o bin/pipe.o

queue: src/queue.c
	gcc -c src/queue.c -o bin/queue.o

socket: src/socket.c
	gcc -c src/socket.c -o bin/socket.o

socket_server: src/socket_server.c
	gcc -c src/socket_server.c -o bin/socket_server.o

clean:
	rm -f bin/*