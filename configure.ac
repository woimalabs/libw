AC_INIT([libw], 0.1.0)
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AM_INIT_AUTOMAKE
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT

CPPFLAGS="$CPPFLAGS -I/usr/lib/sigc++-1.0/include/ -I/usr/include/sigc++-2.0 -I/usr/local/include/sigc++-2.0 -I/usr/local/lib/sigc++-2.0/include"

AC_CONFIG_FILES([Makefile
    src/Makefile
    libw.pc])

AC_OUTPUT

