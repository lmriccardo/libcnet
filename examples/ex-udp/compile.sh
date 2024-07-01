#!/bin/bash

gcc -O3 -Wall -I../../includes -g -o udp-send udp-send.c -L../../dist/static/ -lcnet -lrt
gcc -O3 -Wall -I../../includes -g -o udp-recv udp-recv.c -L../../dist/static/ -lcnet -lrt -lpthread