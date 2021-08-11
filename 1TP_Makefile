all: peggle

CFLAGS=-Wall -std=c99 -pedantic -g -DTTF
LDFLAGS=-lSDL2 -lSDL2_ttf -lm

peggle: test_sdl2.o lista.o obstaculo.o poligono.o leer.o 
	gcc $(CFLAGS) test_sdl2.o lista.o poligono.o obstaculo.o leer.o -o peggle $(LDFLAGS)

test_sdl2.o: test_sdl2.c
	gcc $(CFLAGS) -c test_sdl2.c

lista.o: lista.c lista.h
	gcc $(CFLAGS) -c lista.c

obstaculo.o: obstaculo.c obstaculo.h 
	gcc $(CFLAGS) -c obstaculo.c

poligono.o: poligono.c poligono.h
	gcc $(CFLAGS) -c poligono.c

leer.o: leer.c leer.h
	gcc $(CFLAGS) -c leer.c

clean:
	rm -vf *.o peggle
