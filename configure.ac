AC_INIT([libw], 0.1.0)
AC_CONFIG_MACRO_DIR([build])
AC_CONFIG_AUX_DIR([build])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AM_INIT_AUTOMAKE
AC_ENABLE_SHARED
AC_DISABLE_STATIC
LT_INIT

#PKG_CHECK_MODULES([libsigcpp], [libsigc++])
#AC_SUBST([libsigcpp_CPPFLAGS])
#PKG_CHECK_MODULES([libpng], [libpng])
#AC_SUBST([libpng_CPPFLAGS])

AC_CONFIG_FILES([Makefile
    src/Makefile
    libw.pc])

AC_OUTPUT

