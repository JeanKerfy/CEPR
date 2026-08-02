export PATH := $(CURDIR)/toolchain/bin:$(PATH)

CC = toolchain/bin/gcc.exe
SRC = $(wildcard src/*.c)
OUT = CEPR.exe

CFLAGS = -Wall -Iraylib/include -Isrc
LDFLAGS = -Lraylib/lib -static -lraylib -lopengl32 -lgdi32 -lwinmm

all:
	$(CC) $(CFLAGS) -o $(OUT) $(SRC) $(LDFLAGS)

clean:
	-del $(OUT)