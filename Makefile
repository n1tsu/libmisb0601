CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -I. -Iinclude -Iout/
LIBS=
SRC= \
	src/packet.c					\

OBJS=$(SRC:.c=.o)

all: libmisb0601.so

%.o: %.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS) $(LIBS)

libmisb0601.so: $(OBJS)
	$(CC) $^ --share -o out/$@ $(CFLAGS) $(LIBS)

debug: CFLAGS+= -g
debug: libmisb0601.so

test: libmisb0601.so
	$(CC) test/test.c $(CFLAGS) -Lout -lmisb0601 -o out/test

clean:
	$(RM) out/* $(OBJS) test/test.o

.PHONY: clean all
