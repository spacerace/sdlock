CC=gcc
WARN=-Wall -Werror
INCLUDE=-I./
CFLAGS=-O2 -pipe
LDFLAGS=-lSDL -lSDL_gfx -lSDL_image -lX11
SRC=sdlock.c
OUT=sdlock

all:
	$(CC) $(WARN) $(CFLAGS) $(LDFLAGS) $(INCLUDE) $(SRC) -o $(OUT)

install:
	cp $(OUT) /usr/bin/
	chmod +x /usr/bin/$(OUT)
	echo "please copy sdlock.png to your users home folder as '.sdlock.png'"

