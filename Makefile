GCC_OPTS = -D_DEBUG
all: proxy

debug.o: debug.c
	gcc ${GCC_OPTS} -c debug.c -o debug.o

proxy: proxy.c debug.o
	gcc ${GCC_OPTS} debug.o proxy.c -o proxy
