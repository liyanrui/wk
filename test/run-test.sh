#!/bin/bash

OPT="-std=c11 -pedantic -Wall -g"
MEMCHECK="valgrind --leak-check=yes"

gcc $OPT -I.. ../wk-{err,array,tree}.c tree-test.c -o tree-test
./tree-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,list,box,array,free,v,equal,hash,str,table,cfg}.c cfg-test.c -lm -o cfg-test
./cfg-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,list,box,array,free,v,equal,hash,str,table}.c table-test.c -lm -o table-test
./table-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,array,free,v,equal,hash,str}.c hash-test.c -o hash-test
./hash-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,array,free,v,equal,hash,str}.c v-test.c -o v-test
./v-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,array,free,v,equal,hash,str}.c free-test.c -o free-test
./free-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,list,cli}.c cli-test.c -o cli-test
./cli-test --foo  -ba "hello world" -nm 3 "i am free!"
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,list}.c list-test.c -o list-test
./list-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box}.c box-test.c -o box-test
./box-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,array}.c array-test.c -o array-test
./array-test
echo "-----------------"

gcc $OPT -I.. ../wk-{err,box,array,free,v,equal,hash,str}.c str-test.c -o str-test
./str-test
