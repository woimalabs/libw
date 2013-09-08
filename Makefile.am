SUBDIRS = src

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libw.pc

EXTRA_DIST = autogen.sh

install-exec-hook:
	find $(DESTDIR)$(libdir) -type f -name \*.la -delete
