## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      Makefile.am
## Author:    mgrosso Matthew Grosso
## Created:   
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 Matt Grosso. All Rights Reserved.
## 
## $Id: Makefile.am,v 1.2 2005/10/19 08:44:27 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

LDADD = @LDADD@

APP=cplusql
bin_PROGRAMS = $(APP)

cplusql_SOURCES = cplusql_grammar.y cplusql_lexer.l cplusql_api.cpp CplusqlContext.cpp CplusqlMain.cpp JobQRunnable.cpp ClusterByteSource.cpp ClusterDestFile.cpp ClusterDestFactory.cpp ClusterDestBuffered.cpp CplusqlExceptionHandling.cpp 

AM_YFLAGS= -t -g -d -v -r state,lookahead,itemset,
AM_LFLAGS= -d -8

#cplusql_YFLAGS
#cplusql_LFLAGS
#cplusql_CFLAGS
#cplusql_CPPFLAGS


GRAMMARH = $(APP)_grammar.tab.h
BUILT_SOURCES = $(GRAMMARH)
$(GRAMMARH): cplusql_grammar.o

depend: .depend

.depend: 
	echo >.depend
	makedepend -Y../inc/ -f.depend $(APP_CPP) 2>/dev/null
