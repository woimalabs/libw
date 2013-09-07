AC_INIT([libw], 0.1)
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_PROG_LIBTOOL
AM_INIT_AUTOMAKE

CPPFLAGS="$CPPFLAGS -I/usr/local/include/sigc++-2.0 -I/usr/local/lib/sigc++-2.0/include"

AC_CONFIG_FILES([Makefile
    src/Makefile
    libw.pc])

AC_OUTPUT

