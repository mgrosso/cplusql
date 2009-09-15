#!/bin/sh
PATH=/sbin:/usr/sbin:/bin:/usr/bin:/usr/local/ssl/bin:/usr/local/bin:$PATH

TEST=`type type|grep not`
if test -n "$TEST"; then
  WHICH=which
else
  WHICH=type
fi

echo "On some systems, you may need to change /bin/sh to point to bash"
echo
echo "Please submit the following information with your bug report: "
echo "--------------------------------------------------------------"
uname -s 2>/dev/null |\
	awk '{print "OS             :",$N}'

uname -r 2>/dev/null |\
	awk '{print "OS RELEASE     :",$N}'

uname -m 2>/dev/null |\
	awk '{print "HARDWARE       :",$N}'

TEST=`$WHICH openssl 2>/dev/null`
if test -n "$TEST"; then
  openssl version 2>/dev/null |\
	  awk '/OpenSSL/{print "OpenSSL Version:",$N}'
else
  echo "OpenSSL Version: Not Found"
fi

TEST=`$WHICH gcc 2>/dev/null`
if test -n "$TEST"; then
  gcc --version 2>/dev/null |\
	  awk '{print "gcc version    :",$N}'
else
  echo "gcc version    : Not Found";
fi

TEST=`$WHICH gmake 2>/dev/null`
if test -n "$TEST" ; then
	gmake --version 2>/dev/null |\
		awk -F, '{print $1}' |\
		awk '/GNU Make/{print "Gnu gmake      :",$NF}'
else
  TEST=`make --version 2>/dev/null`
  if test -n "$TEST"; then
		make --version 2>/dev/null |\
			awk -F, '{print $1}' |\
			awk '/GNU Make/{print "Gnu make       :",$NF}'
  else
		echo "Gnu Make       : Not Found"
  fi
fi

TEST=`$WHICH autoconf 2>/dev/null`
if test -n "$TEST"; then
  autoconf --version |\
    head -1 |\
    awk '{\
	if (length($4) == 0) {\
		print "autoconf       : "$3\
	} else {\
		print "autoconf       : "$4\
	}}'
else
  echo "autoconf       : Not Found"
fi

TEST=`$WHICH automake 2>/dev/null`
if test -n "$TEST"; then
  automake --version 2>/dev/null |\
    head -1 |\
    awk '{print "automake       : "$4}'
else
  echo "automake       : Not Found"
fi


TEST=`$WHICH extract 2>/dev/null`
if test -n "$TEST"; then
  extract -v 2>/dev/null |\
    head -1 |\
    awk '{print "libextractor   : "$2}'
else
  echo "libextractor   : Not Found"
fi

TEST=`$WHICH gnunetd 2>/dev/null`
if test -n "$TEST"; then
  gnunetd -v 2>/dev/null |\
    awk '{print "GNUnet         : "$2}'
else
  echo "GNUnet         : Not Found"
fi

TEST=`$WHICH libgcrypt-config 2> /dev/null`
if test -n "$TEST"; then
  libgcrypt-config --version 2> /dev/null | \
    awk '{print "libgcrypt      : "$1}'
else
   echo 'libgcrypt     : Not Found'
fi

echo "--------------------------------------------------------------"
