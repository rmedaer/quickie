AC_INIT([quickie], [1.0], [bug-automake@gnu.org])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AM_PATH_PYTHON([3])
AC_ARG_VAR([PYTHON_INCLUDE], [Include flags for python, bypassing python-config])
AC_ARG_VAR([PYTHON_CONFIG], [Path to python-config])
AS_IF([test -z "$PYTHON_INCLUDE"], [
  AS_IF([test -z "$PYTHON_CONFIG"], [
    AC_PATH_PROGS([PYTHON_CONFIG],
                  [python$PYTHON_VERSION-config python-config],
                  [no],
                  [`dirname $PYTHON`])
    AS_IF([test "$PYTHON_CONFIG" = no], [AC_MSG_ERROR([cannot find python-config for $PYTHON.])])
  ])
  AC_MSG_CHECKING([python cflags])
  PYTHON_CFLAGS=`$PYTHON_CONFIG --cflags`
  AC_SUBST([PYTHON_CFLAGS])
  AC_MSG_RESULT([$PYTHON_CFLAGS])

  AC_MSG_CHECKING([python ldflags])

  # AC_COMPARE_VERSION from ax_compare_version.m4
  # available in autoconf-archive package
  AX_COMPARE_VERSION([$PYTHON_VERSION], [ge], [3.8], [
    PYTHON_LDFLAGS=`$PYTHON_CONFIG --ldflags --embed`
  ], [
    PYTHON_LDFLAGS=`$PYTHON_CONFIG --ldflags`
  ])

  AS_IF([test $? != 0], [AC_MSG_ERROR([error executing $PYTHON_CONFIG])])
  AC_SUBST([PYTHON_LDFLAGS])
  AC_MSG_RESULT([$PYTHON_LDFLAGS])
])

PKG_PROG_PKG_CONFIG

PKG_CHECK_MODULES([CAPNG], [libcap-ng], dummy=yes)
AC_SUBST([CAPNG_CFLAGS])
AC_SUBST([CAPNG_LIBS])

AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_OUTPUT
