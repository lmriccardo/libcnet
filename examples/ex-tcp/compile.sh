#!/bin/bash

gcc -O0 -Wall -I../../includes -g -o tcp tcp.c -L../../dist/static/ -lcnet -lrt
# gcc -O0 -Wall -I../../includes -g -o tcp2 tcp2.c -L../../dist/static/ -lcnet -lrt