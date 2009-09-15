#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


AC_PREREQ(2.58)
AC_INIT([libframe], [1.0.0], [mgrosso@acm.org])

AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([libframe], [1.0.0],[])

AC_CONFIG_SRCDIR([test/test-config.cpp])

AC_PROG_MAKE_SET([])

AC_SUBST(SET_MAKE)

#do I need both of these?
AM_CONFIG_HEADER([inc/system_config.h])

if test x$CFLAGS = x ; then
    CFLAGS="-g"
fi

# Checks for programs.
AC_GNU_SOURCE
AC_PROG_CXX
AC_PROG_CC
#add something to do this later, its not an existing macro...
#AC_PROG_AR
AC_PROG_RANLIB
AC_PROG_INSTALL
# Checks for libraries.
# AC_CHECK_LIB([frame], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([assert.h errno.h fcntl.h float.h glob.h hash_map.h limits.h math.h pthread.h signal.h stdarg.h stdio.h stdlib.h string.h strings.h sys/stat.h sys/types.h time.h time.h unistd.h syslog.h hash_map string vector map list regex.h])

AC_CHECK_HEADERS([pcreposix.h],
    LIBS="$LIBS -lpcreposix -lpcre",
    [true] )

AC_CHECK_HEADERS([pcreposix.h])

# we will need to configure many of these...
#FILENAME_MAX src/DestHashFiles.cpp
#LONG_LONG_MAX src/Convert.cpp
#O_LARGEFILE src/DestFile.cpp src/RawSource.cpp src/fwrap.cpp
#POSTGRES src/DBConnection.cpp
#PTRDEBUG inc/ptr.h
#SEM_VALUE_MAX src/Semaphore.cpp
#_GNU_SOURCE src/Semaphore.cpp

#convert this to a feature test...
#__ARCH_SUN4U__ src/Convert.cpp

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_TYPE_SIGNAL
AC_FUNC_STRTOD
AC_CHECK_FUNCS([dup2 memmove memset strcasecmp strchr strerror strerror_r strpbrk strstr strtol])


#AC_C_LONG_DOUBLE
AC_CXX_HAVE_HASH_MAP
AC_CXX_HAVE_EXT_HASH_MAP
AC_CXX_HAVE_GNU_CXX_EXT_HASH_MAP
AC_CXX_HAVE_GNU_GLOB
AC_CXX_HAVE_CTIME_R
AC_CXX_HAVE_LOGL


FRAME_USES_XML=0
WANT_XML=no
AC_ARG_ENABLE( [libxml2], 
    AC_HELP_STRING([--enable-libxml2], [yes to enable xml related functionality, currently just xpath queries. this requires that libxml2 be installed.]),
    [
    if test "x$enable_xml" = "xyes"
    then 
        WANT_XML=yes
        XMLINC=`xml2-configx --cflags`
        if test $? != 0 ; then
            AC_MSG_ERROR([You have enabled xml but xml2-config either wasnt found or returned an error. Perhaps you dont have libxml2 installed yet?])
        fi
        CPPFLAGS="$CPPFLAGS -I$XMLINC "
        XMLLIBS=" -lxml2 "
        TEST_LDADD="$TEST_LDADD $XMLLIBS "
        LIBS="$LIBS $XMLLIBS "
        FRAME_USES_XML=1
    fi
    ],
    [
        WANT_ACE=no
    ]
)
AC_DEFINE_UNQUOTED(FRAME_USES_XML, $FRAME_USES_XML,
        [Defined to 0, or 1 if you are using libxml2.])


FRAME_USES_ACE=0
WANT_ACE=no
AC_ARG_ENABLE( [ace], 
    AC_HELP_STRING([--enable-ace], [yes to use ACE for cross platform portability; you must also set \$ACE_ROOT.]),
    [
    if test "x$enable_ace" = "xyes"
    then 
        WANT_ACE=yes
        if test "x$ACE_ROOT" = x ; then
            AC_MSG_ERROR([You must set the ACE_ROOT environmental variable to use ace.])
        fi
        CPPFLAGS="$CPPFLAGS -I$ACE_ROOT "
        ACE_LIB=$ACE_ROOT/ace/libACE
        if test -r ${ACE_LIB}.dll.a ; then
            ACE_STATIC_LIB=${ACE_LIB}.dll.a
        else if test -r ${ACE_LIB}.dll ; then
            ACE_SHARED_LIB=${ACE_LIB}.dll
        else if test -r ${ACE_LIB}.so ; then
            ACE_SHARED_LIB=${ACE_LIB}.so
        else if test -r ${ACE_LIB}.a ; then
            ACE_STATIC_LIB=${ACE_LIB}.a
        fi
        fi
        fi
        fi
        if test -z $ACE_SHARED_LIB ; then
            if test -z $ACE_STATIC_LIB ; then
                AC_MSG_ERROR([cant find static or shared ACE lib in ACE_ROOT. Either fix your ace installation, or set ACE_SHARED_LIB or ACE_STATIC_LIB to point to your nonstandard location for this lib.]) 
            fi
            ACE_LIB=$ACE_STATIC_LIB
        else
            ACE_LIB=" -L $ACE_ROOT/ace -lACE "
        fi
        TEST_LDADD="$TEST_LDADD $ACE_LIB "
        #AC_CHECK_HEADER([ace/OS.h],[],[ AC_MSG_ERROR([cant find OS.h in \$ACE_ROOT $ACE_ROOT.]) ])
        FRAME_USES_ACE=1
    fi
    ],
    [
        WANT_ACE=no
    ]
)
AC_DEFINE_UNQUOTED(FRAME_USES_ACE, $FRAME_USES_ACE,
        [Define to 0, or 1 if you have the ace framework.])

FRAME_USES_POSTGRES=0
WANT_POSTGRES=no
AC_ARG_ENABLE( [postgres], 
    AC_HELP_STRING([--enable-postgres], [yes to use POSTGRES for cross platform portability; you must also set \$POSTGRES_HOME.]),
    [
    if test "x$enable_postgres" = "xyes"
    then 
        WANT_POSTGRES=yes
        if test "x$POSTGRES_HOME" = x ; then
            AC_MSG_ERROR([You must set the POSTGRES_HOME environmental variable to use postgres.])
        fi
        CPPFLAGS="$CPPFLAGS -I ${POSTGRES_HOME}/include/"
        POSTGRES_LIB=$POSTGRES_HOME/lib/libpq
        if test -r ${POSTGRES_LIB}.dll.a ; then
            POSTGRES_STATIC_LIB=${POSTGRES_LIB}.dll.a
        else if test -r ${POSTGRES_LIB}.dll ; then
            POSTGRES_SHARED_LIB=${POSTGRES_LIB}.dll
        else if test -r ${POSTGRES_LIB}.so ; then
            POSTGRES_SHARED_LIB=${POSTGRES_LIB}.so
        else if test -r ${POSTGRES_LIB}.a ; then
            POSTGRES_STATIC_LIB=${POSTGRES_LIB}.a
        fi
        fi
        fi
        fi
        if test -z $POSTGRES_SHARED_LIB ; then
            if test -z $POSTGRES_STATIC_LIB ; then
                AC_MSG_ERROR([cant find static or shared POSTGRES lib in POSTGRES_HOME. Either fix your postgres installation, or set POSTGRES_SHARED_LIB or POSTGRES_STATIC_LIB to point to your nonstandard location for this lib.]) 
            fi
            POSTGRES_LIB=$POSTGRES_STATIC_LIB
        else
            POSTGRES_LIB=" -L $POSTGRES_HOME/lib -lpq"
        fi
        TEST_LDADD="$TEST_LDADD $POSTGRES_LIB "
        #AC_CHECK_HEADER([ace/OS.h],[],[ AC_MSG_ERROR([cant find OS.h in \$POSTGRES_HOME $POSTGRES_HOME.]) ])
        FRAME_USES_POSTGRES=1
		LIBS="${LIBS} ${POSTGRES_LIB}"
		#needed? why is AC_DEFINED_UNQUOTED below not working
		DEFS="$DEFS -DFRAME_USES_POSTGRES"
		INC="${INC} $POSTGRES_HOME/include/"
        AC_CHECK_HEADERS([libpq-fe.h])
    fi
    ],
    [
        WANT_POSTGRES=no
    ]
)
AC_DEFINE_UNQUOTED(FRAME_USES_POSTGRES, $FRAME_USES_POSTGRES,
        [Define to 0, or 1 if you have the postgres framework.])

FRAME_USES_ORACLE=0
WANT_ORACLE=no
AC_ARG_ENABLE( [oracle], 
    AC_HELP_STRING([--enable-oracle], [yes to use ORACLE for cross platform portability; you must also set \$ORACLE_HOME.]),
    [
    if test "x$enable_oracle" = "xyes"
    then 
        WANT_ORACLE=yes
        if test "x$ORACLE_HOME" = x ; then
            AC_MSG_ERROR([You must set the ORACLE_HOME environmental variable to use oracle.])
        fi
        CPPFLAGS="$CPPFLAGS -I ${ORACLE_HOME}/rdbms/demo -I ${ORACLE_HOME}/network/public -I ${ORACLE_HOME}/rdbms/public"
        ORACLE_LIB=$ORACLE_HOME/lib/libclntsh
        if test -r ${ORACLE_LIB}.dll.a ; then
            ORACLE_STATIC_LIB=${ORACLE_LIB}.dll.a
        else if test -r ${ORACLE_LIB}.dll ; then
            ORACLE_SHARED_LIB=${ORACLE_LIB}.dll
        else if test -r ${ORACLE_LIB}.so ; then
            ORACLE_SHARED_LIB=${ORACLE_LIB}.so
        else if test -r ${ORACLE_LIB}.a ; then
            ORACLE_STATIC_LIB=${ORACLE_LIB}.a
        fi
        fi
        fi
        fi
        if test -z $ORACLE_SHARED_LIB ; then
            if test -z $ORACLE_STATIC_LIB ; then
                AC_MSG_ERROR([cant find static or shared ORACLE lib in ORACLE_HOME. Either fix your oracle installation, or set ORACLE_SHARED_LIB or ORACLE_STATIC_LIB to point to your nonstandard location for this lib.]) 
            fi
            ORACLE_LIB=$ORACLE_STATIC_LIB
        else
            ORACLE_LIB=" -L $ORACLE_HOME/lib/ -lclntsh -lskgxp8"
        fi
        TEST_LDADD="$TEST_LDADD $ORACLE_LIB "
        #AC_CHECK_HEADER([ace/OS.h],[],[ AC_MSG_ERROR([cant find OS.h in \$ORACLE_HOME $ORACLE_HOME.]) ])
        FRAME_USES_ORACLE=1
		LIBS="${LIBS} ${ORACLE_LIB}"
		#needed? why is AC_DEFINED_UNQUOTED below not working
		DEFS="$DEFS -DFRAME_USES_ORACLE"
    fi
    ],
    [
        WANT_ORACLE=no
    ]
)
AC_DEFINE_UNQUOTED(FRAME_USES_ORACLE, $FRAME_USES_ORACLE,
        [Define to 0, or 1 if you have the oracle framework.])


GCC_VERSION_STRING=`$CXX -v 2>&1 | tail -1 `
GCC_VERSION=`echo $GCC_VERSION_STRING | cut -d ' ' -f 3 `
GCC_MAJOR_VERSION=`echo $GCC_VERSION | cut -d '.' -f 1 `
GCC_MINOR_VERSION=`echo $GCC_VERSION | cut -d '.' -f 2 `

echo $GCC_VERSION_STRING | grep -q mingw >/dev/null 2>&1
if test $? = 0 ; then
    if test x$FRAME_USES_ACE = x0 ; then
        AC_MSG_ERROR([The windows port of libframe requires ACE. add --enable-ace to the configure command line.])
    fi
    CFLAGS=" -mwindows -mthreads $CFLAGS "
fi




#
#ptr configuration
#
WANT_ASM_PTR_SEMAPHORE=yes
AC_ARG_ENABLE( [asm_ptr_semaphore], 
    AC_HELP_STRING([--disable-asm-ptr-semaphore], [if yes, then a slower external semaphore will be used even if we are on an x86(default is no)]),
    [
    if test "x$enable_asm_ptr_semaphore" = "xno"
    then 
        WANT_ASM_PTR_SEMAPHORE=no
    fi
    ],
    [
        WANT_ASM_PTR_SEMAPHORE=yes
    ]
)
PTR_USES_ASM_SEMAPHORE=0
if test "x$WANT_ASM_PTR_SEMAPHORE" = "xyes"
then 
    case $target in
        i386-* | i486-* | i586-* | i686-* | x86_64-* )
          PTR_USES_ASM_SEMAPHORE=1
          if test x$GCC_MAJOR_VERSION = x2 ; then
              CFLAGS="$CFLAGS -fno-inline "
          fi
          ;;
        * )
          undef PTR_USERS_ASM_SEMAPHORE
    ;;
    esac
fi
AC_DEFINE_UNQUOTED(PTR_USES_ASM_SEMAPHORE, $PTR_USES_ASM_SEMAPHORE ,
        [Define to one of 0 or 1, or leave it undefined.])


FRAME_HAS_GNUGLOB=0
SEM_SOLARIS=0
SEM_PTHREAD=0
SEM_ACE=0
if test x$FRAME_USES_ACE = x1 ; then
    if test x$PTR_USES_ASM_SEMAPHORE = x1 ; then
        #dont ever let ace semaphore be used for ptr, since it cant do getvalue.
        FRAME_SEMAPHORE=FRAME_USES_ACE_SEMAPHORE
        SEM_ACE=1
    fi
else
    case $host in 
      *-*-SunOS* )
        FRAME_SEMAPHORE=FRAME_USES_SOLARIS_SEMAPHORE
        SEM_SOLARIS=1
        ;;
      *-*-solaris* )
        FRAME_SEMAPHORE=FRAME_USES_SOLARIS_SEMAPHORE
        SEM_SOLARIS=1
        ;;
        * )
        FRAME_SEMAPHORE=FRAME_USES_PTHREAD_SEMAPHORE
        SEM_PTHREAD=1
        #ideally, the following would be in a system test, not a 
        #per system type variable.
        ;;
    esac
fi

if test x$HAVE_PCREPOSIX_H = x1 ; then
    echo "HAVE_PCREPOSIX_H"
    REGEXLIBS=" -lpcreposix -lpcre "
else
    echo "dont HAVE_PCREPOSIX_H"
fi

AC_DEFINE_UNQUOTED(FRAME_HAS_GNUGLOB, $FRAME_HAS_GNUGLOB,
        [Define to 0, or 1 if you have the postgres framework.])

AC_DEFINE_UNQUOTED([FRAME_USES_SOLARIS_SEMAPHORE],$SEM_SOLARIS,[which kind of semaphore implementation to use])
AC_DEFINE_UNQUOTED([FRAME_USES_ACE_SEMAPHORE],$SEM_ACE,[which kind of semaphore implementation to use])
AC_DEFINE_UNQUOTED([FRAME_USES_PTHREAD_SEMAPHORE],$SEM_PTHREAD,[which kind of semaphore implementation to use])

case $host in 
  *-*-SunOS* )
    SO_OPTS=" -dy -G "
    ;;
  *-*-solaris* )
    SO_OPTS=" -dy -G "
    ;;
  * )
    SO_OPTS=" -shared "
    ;;
esac

case $host in 
  *cygwin* )
    STATIC_ONLY=1
    ;;
  * )
    CFLAGS="$CFLAGS -fPIC "
    PLATFORM_LIBS=-lpthread
    ;;
esac

#this should really be done as a result of a feature test, not hardcoded.
AC_DEFINE(_ISOC99_SOURCE,1,[need this in order to get long double functions])


LIBS="$LIBS $XMLLIBS $REGEXLIBS $PLATFORM_LIBS"
DEFS="$DEFS -D_REENTRANT -D_GNU_SOURCE"
CFLAGS="$CFLAGS -Wall $DEFS $XMLINC "
CPPFLAGS="$CFLAGS $CPPFLAGS"
CXXFLAGS=
SO=so
LIB_BASE=${PACKAGE_NAME}
SHARED_LIB_NO_VERSION=${LIB_BASE}.$SO
SHARED_LIB_W_VERSION=${LIB_BASE}.${SO}.$PACKAGE_VERSION

if test x$STATIC_ONLY = x1 ; then
    TESTLIB=../src/${PACKAGE_NAME}.a
    TEST_LDADD="$TESTLIB $TEST_LDADD "
else
    TESTLIB=../src/${SHARED_LIB}
    TEST_LDADD="$TEST_LDADD -L ../src/ -lframe"
fi

AC_SUBST(TEST_LDADD)
AC_SUBST(TESTLIB)
AC_SUBST(SHARED_LIB_NO_VERSION)
AC_SUBST(SHARED_LIB_W_VERSION)
AC_SUBST(SO_OPTS)

echo target is $target
echo host is $host
echo CFLAGS = $CFLAGS
echo DEFS = $DEFS
echo PTR_USES_ASM_SEMAPHORE = $PTR_USES_ASM_SEMAPHORE
echo FRAME_SEMAPHORE is $FRAME_SEMAPHORE
echo FRAME_USES_POSTGRES is $FRAME_USES_POSTGRES
echo FRAME_USES_ORACLE is $FRAME_USES_ORACLE
echo SO_OPTS are $SO_OPTS
set >.config.env

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])
AC_OUTPUT
