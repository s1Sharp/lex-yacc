ly = lex-yacc/
src = src/
hdr = include/
build = build/
DEBUG ?= 

CC = gcc 
PROGRAM  =  app
CFLAGS = -I$(hdr) -I$(build) -lfl -o 
SRC = $(build)*.c $(src)*.c -lm

.PHONY: all clean run result

all: $(PROGRAM) run
	cat result.txt

y.tab.c: $(ly)yacc.y
	bison -b $(build)y -vd $(ly)yacc.y | yacc -b $(build)y -vd $(ly)yacc.y

lex.yy.c:  $(ly)lex.l
ifeq ($(OS),Windows_NT)
	flex -o $(build)lex.yy.c $(ly)lex.l
else
	lex -o $(build)lex.yy.c $(ly)lex.l 
endif

app: mkdir y.tab.c lex.yy.c
	$(CC) $(SRC) $(CFLAGS) app
	
clean:
	rm -rf $(build)
	rm result.txt
	rm app*

mkdir:
	mkdir -p $(build)

run:
	cat input.txt | ./app $(DEBUG)> result.txt

result:
	cat result.txt


