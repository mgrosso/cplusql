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
## $Id: autoclean.sh,v 1.1 2005/01/03 01:02:35 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

find . -name Makefile -exec rm -f {} \;
find . -name Makefile.in -exec rm -f {} \;
rm -rf inc/config.h aclocal.m4 autom4te.cache configure config.status
