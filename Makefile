all: sandioblast

OBJS := main.o sandio.o evdev.o config.o

INCL ?= -I/usr/include/libevdev-1.0
LIBS ?= -levdev

CFLAGS  ?= -std=c99 -pedantic -Os -Wall -Wextra -fstrict-aliasing
LDFLAGS ?= -s

%.o: %.c Makefile decl.h
	cc -c $(CFLAGS) $(INCL) -o $@ $<

sandioblast: $(OBJS) Makefile
	cc -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

clean:
	-rm -f $(OBJS) sandioblast
