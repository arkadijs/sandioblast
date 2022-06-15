all: sandioblast

OBJS := main.o sandio.o evdev.o config.o

INCL ?= -I/usr/include/libevdev-1.0
LIBS ?= -levdev

CC      ?= cc
CFLAGS  ?= -std=c11 -pedantic -Os -Wall -Wextra
LDFLAGS ?= -s

%.o: %.c Makefile decl.h
	$(CC) -c $(CFLAGS) $(INCL) -o $@ $<

sandioblast: $(OBJS) Makefile
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	-rm -f $(OBJS) sandioblast
