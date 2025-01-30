INCLUDE=-Iinclude/SDL2
LIB=-Llib/SDL2
CFLAGS=-Wall -g -Wextra -std=c11 -pedantic
LFLAGS=-lSDL2
LOCAL=ray.c

all: main

main: main.c ray.c
	clang $(CLAGS) $(LFLAGS) $(INCLUDE) $(LIB) \
	$(LOCAL) -g main.c -o main
