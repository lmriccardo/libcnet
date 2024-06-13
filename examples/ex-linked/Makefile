CC=gcc -Wall -O3
INCLUDES=../../includes/
LIB=../../dist/static/
LIBS=-lpthread -lrt -lcnet

all: clean linked

clean:
	rm -f linked

linked : linked.c
	sudo $(CC) -I$(INCLUDES) -g -o linked linked.c -L$(LIB) $(LIBS)
	sudo chmod u+s linked

memcheck: linked.c
	sudo $(CC) -I$(INCLUDES) -g -o linked linked.c -L$(LIB) $(LIBS)

.PHONY: clean