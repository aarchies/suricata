
dnl ----------------------
dnl Initialization macros
dnl ----------------------

AC_INIT([LibHTP],[m4_esyscmd(./get-version.sh VERSION)])
AM_INIT_AUTOMAKE()

AC_CONFIG_HEADERS([htp_config_auto_gen.h])
AC_CONFIG_FILES([htp/htp_version.h])


dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

GENERIC_LIBRARY_NAME=htp

# This is _NOT_ the library release version, it's an API version.
GENERIC_LIBRARY_VERSION=2:0:0
#			| | |
#		 +------+ | +---+
#		 |        |     |
#		current:revision:age
#		 |        |     |
#		 |        |     +- increment if interfaces have been added
#		 |        |	   set to zero if interfaces have been removed
#		 |	  |	   or changed
#		 |	  +- increment if source code has changed
#		 |	     set to zero if current is incremented
#		 +- increment if interfaces have been added, removed or changed
AC_SUBST(GENERIC_LIBRARY_VERSION)

dnl --------------------------------
dnl Package name and version number
dnl --------------------------------

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$PACKAGE_VERSION
GENERIC_RELEASE=$PACKAGE_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

AC_CONFIG_MACRO_DIR([m4])

dnl --------------------------------
dnl Options
dnl --------------------------------

AC_ARG_ENABLE(debug, [  --enable-debug  Enable debug mode],, [ enable_debug=no ])
if test "x$enable_debug" = "xyes"; then
    CFLAGS="${CFLAGS} -DHTP_DEBUG"
    echo "Debug mode enabled"
fi

OLEVEL=2

AC_ARG_ENABLE(devmode, [  --enable-devmode  Enable development mode],, [ enable_devmode=no ])
if test "$enable_devmode" = "yes"; then
    OLEVEL=0
    CFLAGS="${CFLAGS} -Werror -Wfatal-errors"
    CPPFLAGS="${CPPFLAGS} -Werror -Wfatal-errors"
    echo "Development mode enabled"
fi

AC_ARG_ENABLE(gcov, [  --enable-gcov  Enable gcov support],, [ enable_gcov=no ])
if test "$enable_gcov" = "yes"; then
    OLEVEL=0
    CFLAGS="${CFLAGS} --coverage -fprofile-arcs -ftest-coverage"
    CPPFLAGS="${CPPFLAGS} --coverage -fprofile-arcs -ftest-coverage"
    LDFLAGS="${LDFLAGS} -lgcov --coverage -fprofile-arcs"
    echo "gcov support enabled"
fi

CFLAGS="${CFLAGS} -O${OLEVEL}"
CPPFLAGS="${CPPFLAGS} -O${OLEVEL}"

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CXX
LT_INIT
AM_SANITY_CHECK

# Checks for library functions
#AC_CHECK_FUNCS([strlcpy strlcat])
    OCFLAGS=$CFLAGS
    CFLAGS=""
    AC_CHECK_FUNCS([strlcpy strlcat])
    CFLAGS=$OCFLAGS

dnl -----------------------------------------------
dnl Checks for libs.
dnl -----------------------------------------------
AC_CHECK_HEADER(zlib.h,,[AC_MSG_ERROR(zlib.h not found ...)])
ZLIB=""
AC_CHECK_LIB(z, inflate,, ZLIB="no")
if test "$ZLIB" = "no"; then
    echo
    echo "   ERROR! zlib library not found"
    echo
    exit 1
fi

# Determine the OS 
AC_MSG_CHECKING([OS])
OS=`uname -s`
case "$OS" in
        MINGW*)
                AC_MSG_RESULT(MinGW)
                OS_WINDOWS="true"
                NO_STACK_PROTECTOR="true"
        ;;
        MSYS*)
                AC_MSG_RESULT(MSYS)
                OS_WINDOWS="true"
                NO_STACK_PROTECTOR="true"
        ;;
        CYGWIN*)
                AC_MSG_RESULT(Cygwin)
                OS_CYGWIN="true"
                NO_STACK_PROTECTOR="true"
        ;;
        FreeBSD*)
                AC_MSG_RESULT(FreeBSD)
                OS_FREEBSD="true"
                CPPFLAGS="${CPPFLAGS} -I/usr/local/include"
                LDFLAGS="${LDFLAGS} -L/usr/local/lib"
        ;;
        OpenBSD*)
                AC_MSG_RESULT(OpenBSD)
                OS_OPENBSD="true"
                CPPFLAGS="${CPPFLAGS} -I/usr/local/include"
                LDFLAGS="${LDFLAGS} -L/usr/local/lib"
        ;;
        Linux*)
                AC_MSG_RESULT(Linux)
                OS_LINUX="true"
        ;;
        *)
                AC_MSG_RESULT(no)
        ;;
esac
#We need to call the iconv macro after OS detection for FreeBSD to work properly
sinclude(m4/iconv.m4)
sinclude(m4/lib-ld.m4)
sinclude(m4/lib-link.m4)
sinclude(m4/lib-prefix.m4)
AM_ICONV
AM_CONDITIONAL([CYGWIN], [test x${OS_CYGWIN} = xtrue])

# iconvctl is not standard, it is defined only in GNU libiconv
AC_MSG_CHECKING(for iconvctl)
TMPLIBS="${LIBS}"
LIBS="${LIBS} ${LIBICONV}"

AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
             #include <iconv.h>]], [[int iconv_param = 0;
             iconv_t cd = iconv_open("","");
             iconvctl(cd, ICONV_SET_DISCARD_ILSEQ, &iconv_param);
             iconv_close(cd);]])],[ac_cv_func_iconvctl=yes],[])
AC_MSG_RESULT($ac_cv_func_iconvctl)
if test "$ac_cv_func_iconvctl" == yes; then
    AC_DEFINE(HAVE_ICONVCTL,1,"Define to 1 if you have the `iconvctl' function.")
fi
LIBS="${TMPLIBS}"

dnl -----------------------------------------------
dnl Check and enable the GCC opts we want to use.
dnl We may need to add more checks
dnl -----------------------------------------------

dnl -----------------------------------------------
dnl Check for GCC signed overflow warning support
dnl -----------------------------------------------
AC_MSG_CHECKING(for gcc support of -Wstrict-overflow=1)
TMPCFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -Wstrict-overflow=1"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[gcc_have_strict_overflow=yes],[gcc_have_strict_overflow=no])
AC_MSG_RESULT($gcc_have_strict_overflow)
if test "$gcc_have_strict_overflow" != "yes"; then
    CFLAGS="${TMPCFLAGS}"
fi

if test "$NO_STACK_PROTECTOR" != "true"; then
dnl -----------------------------------------------
dnl Check for GCC stack smashing protection
dnl -----------------------------------------------
AC_MSG_CHECKING(for gcc support of stack smashing protection)
TMPCFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -fstack-protector"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[gcc_have_fstack_protector=yes],[gcc_have_fstack_protector=no])
AC_MSG_RESULT($gcc_have_fstack_protector)
if test "$gcc_have_fstack_protector" != "yes"; then
    CFLAGS="${TMPCFLAGS}"  
fi
fi

dnl -----------------------------------------------
dnl Check for GCC -D_FORTIFY_SOURCE support
dnl -----------------------------------------------
AC_MSG_CHECKING(for gcc support of FORTIFY_SOURCE)
TMPCFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -D_FORTIFY_SOURCE=2"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[gcc_have_fortify_source=yes],[gcc_have_fortify_source=no])
AC_MSG_RESULT($gcc_have_fortify_source)
if test "$gcc_have_fortify_source" != "yes"; then
    CFLAGS="${TMPCFLAGS}"
fi

dnl -----------------------------------------------
dnl Check for GCC -Wformat-security support
dnl -----------------------------------------------
AC_MSG_CHECKING(for gcc support of -Wformat -Wformat-security)
TMPCFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -Wformat -Wformat-security"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[gcc_have_format_security=yes],[gcc_have_format_security=no])
AC_MSG_RESULT($gcc_have_format_security)
if test "$gcc_have_format_security" != "yes"; then
    CFLAGS="${TMPCFLAGS}"
fi

AC_MSG_CHECKING(for gcc support of -fPIC)
TMPCFLAGS="${CFLAGS}"
CFLAGS="${CFLAGS} -fPIC"
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[]])],[gcc_have_fpic=yes],[gcc_have_fpic=no])
AC_MSG_RESULT($gcc_have_fpic)
if test "$gcc_have_fpic" != "yes"; then
    CFLAGS="${TMPCFLAGS}"
fi

dnl -----------------------------------------------
dnl Check for doxygen
dnl -----------------------------------------------
AC_ARG_WITH([doxygen],
            [  --with-doxygen=PROG doxygen executable],
            [doxygen="$withval"],[doxygen=no])

if test "$doxygen" != "no"; then
    AC_MSG_NOTICE([Using doxygen: $doxygen])
else
    AC_PATH_PROGS([doxygen],[doxygen],[])
fi

DOXYGEN=$doxygen
AC_SUBST(DOXYGEN)

dnl -----------------------------------------------
dnl Check for lcov
dnl -----------------------------------------------
AC_PATH_PROG(LCOV, lcov, [no])
AC_SUBST(LCOV)


dnl -----------------------------------------------
dnl Generates Makefiles, configuration files and scripts
dnl -----------------------------------------------

AC_PREFIX_DEFAULT(/usr/local)
AC_CONFIG_FILES([Makefile \
          htp.pc \
          htp/Makefile \
          htp/lzma/Makefile \
          test/Makefile \
          docs/Makefile
])
AC_OUTPUT
