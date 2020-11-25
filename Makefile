CC?=gcc
CFLAGS+=-Wall -Werror -Wextra -pedantic -I. -Iinclude -fPIC -L. -lm
LIBNAME=libmisb0601.so
TESTNAME=misb0601-test
SRC= \
	src/packet.c					\
	src/unpack.c					\
	src/utils.c						\

OBJS=$(SRC:.c=.o)

all: $(LIBNAME)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(LIBNAME): $(OBJS)
	$(CC) $^ -shared -o $@ $(CFLAGS)

debug: CFLAGS+= -g
debug: $(LIBNAME)

install: $(LIBNAME)
	install -d 0755 ${DESTDIR}/usr/lib $(DESTDIR)/usr/bin $(DESTDIR)/usr/include/misb0601
	install -m 0644 $(LIBNAME) $(DESTDIR)/usr/lib/$(LIBNAME)
	install -m 0644 include/*.h $(DESTDIR)/usr/include/misb0601/

$(TESTNAME):
	$(CC) test/test.c $(CFLAGS) -g -lmisb0601 -o $@

clean:
	$(RM) $(TESTNAME) $(LIBNAME) $(OBJS) test/test.o

.PHONY: clean all
