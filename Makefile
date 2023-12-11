all: main
	bin/main

main: bin/queue.o bin/worker.o bin/master.o bin/client.o bin/main.o
	gcc -o bin/main bin/main.o

bin/main.o: src/main.c
	gcc -c src/main.c -o bin/main.o

bin/client.o: src/client.c
	gcc -c src/client.c -o bin/client.o

bin/master.o: src/node/master.c
	gcc -c src/node/master.c -o bin/master.o

bin/worker.o: src/node/worker.c
	gcc -c src/node/worker.c -o bin/worker.o

bin/queue.o: src/util/queue.c
	gcc -c src/util/queue.c -o bin/queue.o

clean:
	rm -f bin/*