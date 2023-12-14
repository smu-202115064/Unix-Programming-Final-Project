all: pipe queue socket socket_server

client: src/client.c pipe
	gcc -c src/client.c -o bin/client src/pipe.o

master: src/client.c
	gcc -c src/master.c

worker: src/client.c
	gcc -c src/worker.c

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