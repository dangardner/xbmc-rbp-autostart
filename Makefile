CC:=gcc
CPPFLAGS:=$(shell dpkg-buildflags --get CPPFLAGS)
CFLAGS:=$(shell dpkg-buildflags --get CFLAGS)
LDFLAGS:=$(shell dpkg-buildflags --get LDFLAGS)
SOURCES=vt-helper.c
BINARY=vt-helper
LIBDIR=/usr/lib/xbmc

DPKG_EXPORT_BUILDFLAGS = 1

include /usr/share/dpkg/buildflags.mk

all: $(SOURCES) $(BINARY)

install: $(BINARY)
	install -d $(DESTDIR)$(LIBDIR)
	install -s $(BINARY) $(DESTDIR)$(LIBDIR)
	install -m0755 xbmc-watchdog $(DESTDIR)$(LIBDIR)

clean:
	rm -f $(BINARY)
	
$(BINARY):
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $@
