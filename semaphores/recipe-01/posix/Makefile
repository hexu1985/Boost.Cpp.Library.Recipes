CC = gcc
CFLAGS = -g3 -Wall -Wextra -Ilib

LIB_HDRS = $(shell ls lib/*.h)
LIBRARY = lib/libtlpi.a

SOURCES = $(shell ls *.c)
PROGRAMS = $(patsubst %.c,%,$(SOURCES))

.PHONY: all
all: $(PROGRAMS)

$(PROGRAMS): $(LIBRARY)
$(PROGRAMS): %: %.o
	$(CC) -o $@ $^ $(LIBRARY)

$(LIBRARY): T := all
$(LIBRARY): libs_everything

libs_everything:
	$(MAKE) $(T) --directory=lib

%.o: %.c $(LIB_HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY:
clean: T := clean
clean: libs_everything
	$(RM) $(PROGRAMS) *.o
