CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -I. -Iinclude
LIBS=
SRC= \
	src/packet.c					\

OBJS=$(SRC:.c=.o)

all: misb0601lib.so

%.o: %.c
	$(CC) -c -fPIC -o $@ $< $(CFLAGS) $(LIBS)

misb0601lib.so: $(OBJS)
	$(CC) $^ --share -o out/$@ $(CFLAGS) $(LIBS)

debug: CFLAGS+= -g
debug: misb0601lib.so

clean:
	$(RM) out/* $(OBJS)

.PHONY: clean all
