AC_INIT([libw], 0.1.0)
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_HEADERS([config.h])
AX_CXX_COMPILE_STDCXX_0X
AC_PROG_CXX
AM_INIT_AUTOMAKE
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT

CPPFLAGS="$CPPFLAGS \
    -I/usr/local/include/sigc++-2.0 \
    -I/usr/local/lib/sigc++-2.0/include \
    -I/usr/include/sigc++-2.0/ \
    -I/usr/local/include/eigen3 \
    -I/usr/include/eigen3/ \
    -L/usr/local/lib"

AC_CONFIG_FILES([Makefile
    src/Makefile
    libw.pc])

AC_OUTPUT

