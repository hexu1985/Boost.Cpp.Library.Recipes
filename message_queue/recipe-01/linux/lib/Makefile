CC = gcc
CFLAGS = -Wall -g3
AR = ar
ARFLAGS = rcs
LIBRARY = libtlpi.a

SOURCES = $(shell ls *.c)
HEADERS = $(shell ls *.h)
OBJECTS = $(patsubst %.c,%.o,$(SOURCES))

.PHONY: all 
all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $(LIBRARY) $(OBJECTS)

%.o: %.c $(HEADERS) ename.c.inc
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: ename.c.inc
ename.c.inc:
	sh Build_ename.sh > ename.c.inc
	echo 1>&2 "ename.c.inc built"

.PHONY: clean
clean:
	rm -f ename.c.inc $(OBJECTS) $(LIBRARY)

