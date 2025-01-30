INCLUDE=-Iinclude/SDL2 -Iinclude/SDL2_image
LIB=-Llib/SDL2 -Llib/SDL2_image
CFLAGS=-Wall -g -Wextra -std=c11 -pedantic
LFLAGS=-lSDL2 -lSDL2_image
LOCAL=ray.c

all: main

main: main.c ray.c
	clang $(CLAGS) $(LFLAGS) $(INCLUDE) $(LIB) \
	$(LOCAL) -g main.c -o main
