## Process this file with automake to produce Makefile.in
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      Makefile.am
## Author:    mgrosso Matthew E Grosso
## Created:   Sun Dec 14 20:12:07 EST 2003 on erasmus.erasmus.org
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 Matthew E Grosso. All Rights Reserved.
## 
## $Id: Makefile.am,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

INCLUDES = -I$(top_srcdir)/inc
#SUBDIRS  = idl services src test 
SUBDIRS  = @SERVICESUBDIRS@ src test 
EXTRA_DIST = inc test

dist-hook:
	rm -rf `find $(distdir)/inc/ -name CVS`
	rm -rf $(distdir)/inc/system_config.h
	rm -rf `find $(distdir)/test/ -name CVS`
	rm -rf `find $(distdir)/test/ -name "*.err"`
	rm -rf `find $(distdir)/test/ -name "*.out"`
	rm -rf `find $(distdir)/test/ -name "*.diff"`
	rm -rf `find $(distdir)/test/ -name "*.o"`
	rm -rf `find . -name "test_*" -a ! -name "*_skip" -a ! -name "*\.eng"`
