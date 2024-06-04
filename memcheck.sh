#!/bin/bash 

rm -f main-vg
make main-vg
sudo valgrind --leak-check=full --show-leak-kinds=all -v ./main-vg