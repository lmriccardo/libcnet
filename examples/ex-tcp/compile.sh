#!/bin/bash

gcc -O3 -Wall -I../../includes -g -o tcp tcp.c -L../../dist/static/ -lcnet -lrt