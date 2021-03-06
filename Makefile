CC?=gcc
CFLAGS+=-Wall -Werror -Wextra -pedantic -I. -Iinclude -fPIC -L. -lm
LIBNAME=libmisb0601.so
TESTNAME=unit-test
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

$(TESTNAME): test/test.c
	$(CC) test/test.c $(CFLAGS) -g -lmisb0601 -lcriterion -o $@

clean:
	$(RM) $(TESTNAME) $(LIBNAME) $(OBJS) test/test.o klv_test.bin

.PHONY: clean all
