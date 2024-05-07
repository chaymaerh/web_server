SOURCES = $(wildcard *.c)
OBJETS = $(SOURCES:.c=.o)
CIBLE = libReseau.a

#
# Nom de la cible principale
#

all: $(CIBLE)

clean:
	rm -f core *.o $(CIBLE)

$(CIBLE): $(OBJETS)
	$(AR) rs $@ $?

$(CIBLE:.a=).o: $(CIBLE:.a=).c $(CIBLE:.a=).h
