CC=gcc -Wall -O3
INCLUDES=../includes/
LIB=../dist/static/

all: clean ping

clean:
	rm -f ping

ping : ping.c
	sudo $(CC) -I$(INCLUDES) -g -o ping ping.c -L$(LIB) -lcnet
	sudo chmod u+s ping

memcheck: ping.c
	sudo $(CC) -I$(INCLUDES) -g -o ping ping.c -L$(LIB) -lcnet

.PHONY: clean