all: sandioblast

OBJS := main.o sandio.o evdev.o config.o

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

SYSTEMD_UNIT_DIR ?= /etc/systemd/system
UDEV_RULES_DIR   ?= /etc/udev/rules.d

INCL ?= -I/usr/include/libevdev-1.0
LIBS ?= -levdev

CC      ?= cc
CFLAGS  ?= -std=c11 -pedantic -Os -Wall -Wextra

PAGESIZE := $(shell getconf PAGESIZE 2>/dev/null)
ifneq ($(PAGESIZE),)
LDFLAGS ?= -s -Wl,-z,max-page-size=$(PAGESIZE)
else
LDFLAGS ?= -s
endif

%.o: %.c Makefile decl.h
	$(CC) -c $(CFLAGS) $(INCL) -o $@ $<

sandioblast: $(OBJS) Makefile
	$(CC) -o $@ $(LDFLAGS) $(OBJS) $(LIBS)

install: sandioblast
	install -d $(DESTDIR)$(BINDIR)
	install -m 755 sandioblast $(DESTDIR)$(BINDIR)/sandioblast

uninstall:
	rm -f $(DESTDIR)$(BINDIR)/sandioblast

install-systemd:
	install -d $(DESTDIR)$(SYSTEMD_UNIT_DIR)
	install -m 644 sandioblast@.service $(DESTDIR)$(SYSTEMD_UNIT_DIR)/sandioblast@.service
	install -d $(DESTDIR)$(UDEV_RULES_DIR)
	install -m 644 99-sandio.rules $(DESTDIR)$(UDEV_RULES_DIR)/99-sandio.rules

uninstall-systemd:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/sandioblast@.service
	rm -f $(DESTDIR)$(UDEV_RULES_DIR)/99-sandio.rules

clean:
	-rm -f $(OBJS) sandioblast

.PHONY: all clean install uninstall install-systemd uninstall-systemd
