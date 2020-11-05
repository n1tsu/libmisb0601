CC?=gcc
CFLAGS+=-Wall -Werror -Wextra -pedantic -I. -Iinclude -Iout/ -fPIC
LIBNAME=libmisb0601.so
TESTNAME=misb0601-test
SRC= \
	src/packet.c					\
	src/unpack.c					\

OBJS=$(SRC:.c=.o)

all: $(LIBNAME) $(TESTNAME)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIBNAME): $(OBJS)
	mkdir -p out
	$(CC) $^ -shared -o out/$@ $(CFLAGS)

debug: CFLAGS+= -g
debug: $(LIBNAME)

install: $(LIBNAME) $(TESTNAME)
	install -d 0755 ${DESTDIR}/usr/lib $(DESTDIR)/usr/bin $(DESTDIR)/usr/include/misb0601
	install -m 0644 out/$(LIBNAME) $(DESTDIR)/usr/lib/$(LIBNAME)
	install -m 0644 out/$(TESTNAME) $(DESTDIR)/usr/bin/$(TESTNAME)
	install -m 0644 include/packet.h $(DESTDIR)/usr/include/misb0601/packet.h
	install -m 0644 include/keys.h $(DESTDIR)/usr/include/misb0601/keys.h

$(TESTNAME):
	$(CC) test/test.c $(CFLAGS) -g -Lout -lmisb0601 -o out/$@

clean:
	$(RM) out/* $(OBJS) test/test.o

.PHONY: clean all
