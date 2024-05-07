export CC = gcc
export LD = gcc
export CLIB = ar cq
export CFLAGS = -Wall
export CDEBUG = -g -DDEBUG

#
# Constantes liees au projet
#

DIRS= Libs Sioux  #EDR

#
# La cible generale
#

all: $(patsubst %, _dir_%, $(DIRS))

$(patsubst %,_dir_%,$(DIRS)):
	cd $(patsubst _dir_%,%,$@) && make

#
# La cible de nettoyage
#

clean: $(patsubst %, _clean_%, $(DIRS))

$(patsubst %,_clean_%,$(DIRS)):
	cd $(patsubst _clean_%,%,$@) && make clean
