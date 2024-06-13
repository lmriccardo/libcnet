CC=gcc -Wall -O3
INCLUDES=../../includes/
LIB=../../dist/static/
LIBS=-lpthread -lrt -lcnet

all: clean ping

clean:
	rm -f ping

ping : ping.c
	sudo $(CC) -I$(INCLUDES) -g -o ping ping.c -L$(LIB) $(LIBS)
	sudo chmod u+s ping

memcheck: ping.c
	sudo $(CC) -I$(INCLUDES) -g -o ping ping.c -L$(LIB) $(LIBS)

.PHONY: clean