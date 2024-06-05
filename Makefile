CC=gcc
OBJECTS=$(patsubst src/%.c,./%.o,$(wildcard src/*.c))
LIBS=-lpthread
SRC=$(wildcard src/*.c)

# the last operation executed by the makefile is to set the SUID
# bit to the main executable file. In this way, even if it is 
# executed by a low-level privileged user, it gains root privileges.

all: main clean

main: $(OBJECTS)
	sudo $(CC) -I./includes $(LIBS) -o dist/$@ $^
	sudo chmod u+s dist/$@
	rm -f main
	sudo ln -s dist/$@ $@

$(OBJECTS): %.o: src/%.c
	sudo $(CC) -I./includes $(LIBS) -c -o $@ $<

clean:
	rm -f $(OBJECTS)

debug:
	rm -f debug
	sudo $(CC) -I./includes $(LIBS) -g -o $@ $(SRC)
	sudo chmod u+s $@

main-vg:
	sudo $(CC) -I./includes $(LIBS) -o $@ $(SRC)

.PHONY: clean