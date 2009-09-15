#!/bin/bash  -x
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      auto.sh
## Author:    mgrosso Matthew E Grosso
## Created:   Wed Dec 17 01:50:20 EST 2003 on erasmus.erasmus.org
## Project:   
## Purpose:   
## 
## Copyright (c) 2003 Matthew E Grosso. All Rights Reserved.
## 
## $Id: auto.sh,v 1.1 2005/10/17 11:18:58 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

./autoclean.sh
#cd test/ ; ./make_makefile_am.sh ; cd  -
aclocal || exit 1;
autoheader || exit 1;
automake -i  || exit 1;
autoconf || exit 1;
