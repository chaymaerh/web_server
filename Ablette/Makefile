TARGET= projet_ablette

CFLAGS=-Wall

LDFLAGS=-lm -lpcap


default: $(TARGET)


ablette.o: ablette.c ablette.h
	gcc $(CFLAGS) -c ablette.c


$(TARGET): ablette.o
	gcc ablette.o -o $(TARGET) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f *.o
	rm -f $(TARGET)
