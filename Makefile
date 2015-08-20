CC=cc -fPIC -g -O0 -Wall
LD=cc -rdynamic
PREFIX=./run
INCLUDES=-I./include -I./include/ircd

all: src/main.o src/module.o src/irc_cmp.o run/troutfin modules
	mkdir -p run
	${LD} -o run/troutfin src/module.o src/main.o src/irc_cmp.o -ldl

src/main.o: src/main.c
	${CC} -c -o src/main.o src/main.c ${INCLUDES}

src/module.o: src/module.c
	${CC} -c -o src/module.o src/module.c ${INCLUDES}

src/irc_cmp.o: src/irc_cmp.c
	${CC} -c -o src/irc_cmp.o src/irc_cmp.c ${INCLUDES}

clean:
	@rm src/main.o src/module.o modules/*.so

include ./modules/Makefile

