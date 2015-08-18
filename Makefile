PREFIX=./run

all: src/main.o src/module.o
	mkdir -p run
	cc -o run/troutfin src/module.o src/main.o

src/main.o: src/main.c
	cc -c -o src/main.o src/main.c -I./include

src/module.o: src/module.c
	cc -c -o src/module.o src/module.c -I./include

clean:
	@rm src/main.o src/module.o
