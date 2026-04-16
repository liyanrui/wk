.PHONY: all install uninstall clean

prefix := /usr/local
static_objects := $(subst src/,build/,$(subst .c,.o,$(wildcard src/wk-*.c)))
shared_objects := $(subst src/,build/,$(subst .c,-pic.o,$(wildcard src/wk-*.c)))

all: build/libwk.a build/libwk.so

build/libwk.a: $(static_objects)
	ar rcs $@ $^
build/%.o: src/%.c
	gcc -std=c11 -pedantic -Wall -I./src -c $< -o $@

build/libwk.so: $(shared_objects)
	gcc -shared $^ -o $@

build/%-pic.o: src/%.c
	gcc -I./src -fPIC -c $< -o $@


install:
	install -m 644 src/wk-*.h $(prefix)/include
	install -m 644 build/libwk.a $(prefix)/lib
	install -m 644 build/libwk.so $(prefix)/lib

uninstall:
	rm $(prefix)/include/wk-*.h
	rm $(prefix)/lib/libwk.a
	rm $(prefix)/lib/libwk.so

clean:
	rm build/*.o
	rm build/libwk.*
