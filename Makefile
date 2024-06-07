CC=gcc -Wall -O3
OBJECTS=$(patsubst src/%.c,./%.o,$(wildcard src/*.c))
LIBS=-lpthread
SRC=$(wildcard src/*.c)

# the last operation executed by the makefile is to set the SUID
# bit to the main executable file. In this way, even if it is 
# executed by a low-level privileged user, it gains root privileges.

all: main clean

main: $(OBJECTS)
	sudo $(CC) -I./includes $(LIBS) -o $@ main.c $^
	sudo chmod u+s $@

static: $(OBJECTS)
	ar rcsv dist/static/libcnet.a $^
	rm -f $(OBJECTS)

shared: $(OBJECTS) 
	$(CC) -shared -o dist/shared/libcnet.so $^
	rm -f $(OBJECTS)

$(OBJECTS): %.o: src/%.c
	sudo $(CC) -I./includes/ $(LIBS) -fPIC -c -o $@ $<

clean:
	rm -f $(OBJECTS)

debug:
	rm -f debug
	sudo $(CC) -I./includes $(LIBS) -g -o $@ main.c $(SRC)
	sudo chmod u+s $@

main-vg:
	sudo $(CC) -I./includes $(LIBS) -o $@ main.c $(SRC)

.PHONY: clean