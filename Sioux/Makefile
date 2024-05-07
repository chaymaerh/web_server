TARGET = projet
CC = gcc
CFLAGS = -Wall -lpthread

LDFLAGS=-lm

default: $(TARGET)


thread.o: thread.c thread.h
	gcc $(CFLAGS) -c thread.c

analyste_http.o: analyste_http.c analyste_http.h
	gcc $(CFLAGS) -c analyste_http.c


port.o: port.c port.h
	gcc $(CFLAGS) -c port.c


sioux.o: sioux.c port.h analyste_http.h ../Libs/Reseau/libReseau.h thread.h
	gcc $(CFLAGS) -c sioux.c


$(TARGET): sioux.o port.o analyste_http.o thread.o
	gcc sioux.o port.o analyste_http.o ../Libs/Reseau/libReseau.o thread.o -o $(TARGET) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
