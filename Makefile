CC=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -I. -Iinclude
LIBS=
SRC= \
	src/packet.c					\

OBJS=$(SRC:.cpp=.o)

all: misb0601lib.so

%.o: %.c
	$(CXX) -c -o $@ $< $(CFLAGS) $(LIBS)

misb0601lib.so: $(OBJS)
	$(CC) $^ --share -o out/$@ $(CFLAGS) $(LIBS)

clean:
	$(RM) out/* $(OBJS)

.PHONY: clean all
