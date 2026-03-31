.PHONY: all install uninstall clean

prefix = /usr/local
static_objects = $(subst .c,.o,$(wildcard wk-*.c))
shared_objects = $(subst .c,-pic.o,$(wildcard wk-*.c))

all: libwk.a libwk.so

libwk.a: $(static_objects)
	ar rcs $@ $^
%.o: %.c
	gcc -I. -c $< -o $@

libwk.so: $(shared_objects)
	gcc -shared $^ -o $@

%-pic.o: %.c
	gcc -I. -fPIC -c $< -o $@

install:
	install -m 644 wk-*.h $(prefix)/include
	install -m 644 libwk.a libwk.so $(prefix)/lib

uninstall:
	rm $(prefix)/include/wk-*.h
	rm $(prefix)/lib/libwk.{a,so}

clean:
	rm *.o libwk.so libwk.a
