CC=gcc -Wall -O3
OBJECTS=$(patsubst src/%.c,bin/%.o,$(wildcard src/*.c))
LIBS=-lpthread -lrt
SRC=$(wildcard src/*.c)

# the last operation executed by the makefile is to set the SUID
# bit to the main executable file. In this way, even if it is 
# executed by a low-level privileged user, it gains root privileges.

static: $(OBJECTS)
	ar rcsv dist/static/libcnet.a $^

shared: $(OBJECTS) 
	$(CC) -shared -o dist/shared/libcnet.so $^

$(OBJECTS): bin/%.o: src/%.c
	sudo $(CC) -I./includes/ $(LIBS) -fPIC -g -c -o $@ $<

clean:
	rm -f $(OBJECTS)

.PHONY: clean