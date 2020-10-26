CC?=gcc
CFLAGS=-Wall -Werror -Wextra -pedantic -I. -Iinclude -Iout/ -fPIC
LIBNAME=libmisb0601.so
HEADER=packet.h
TESTNAME=misb0601-test
SRC= \
	src/packet.c					\

OBJS=$(SRC:.c=.o)

all: $(LIBNAME) test

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIBNAME): $(OBJS)
	mkdir -p out
	$(CC) $^ --share -o out/$@ $(CFLAGS)

debug: CFLAGS+= -g
debug: $(LIBNAME)

install: $(LIBNAME)
	install -d 0755 ${DESTDIR}/usr/lib $(DESTDIR)/usr/bin $(DESTDIR)/usr/include/misb0601
	install -m 0644 $(LIBNAME) $(DESTDIR)/usr/lib/$(LIBNAME)
	install -m 0644 $(TESTNAME) $(DESTDIR)/usr/bin/$(TESTNAME)
	install -m 0644 include/$(HEADER) $(DESTDIR)/usr/include/misb0601/$(HEADER)

$(TESTNAME): debug
	$(CC) test/test.c $(CFLAGS) -Lout -lmisb0601 -o out/$@

clean:
	$(RM) out/* $(OBJS) test/test.o

.PHONY: clean all
