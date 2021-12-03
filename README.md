# How to use Makefile 

### Makefile 
    make        - call build app and run your test from input.txt
    make all    - make exe file
    make run    - test your program with input.txt (see output in result.txt)
    make result - see tests result in console
    make clean  - clean all build and tmp files 

### Optional build dependencies 
    DEBUG=-d  - to start with debug flag  ( by default off ) 
    FILE="input filename | filepath"  (by default input.txt)

### Example
    make FILE=infile.txt
    make FILE=debug.txt DEBUG=-d
    make clear
# 
![alt text](https://github.com/s1Sharp/lex-yacc/blob/master/task.png?raw=true)
