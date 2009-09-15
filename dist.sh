#!/bin/bash
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------
##
## File:      ptr.sh
## Author:    mgrosso Matthew Grosso
## Created:   Sat Jan 10 15:03:08 EST 2004 on localhost.localdomain
## Project:   
## Purpose:   
## 
## Copyright (c) 2004 LookSmart. All Rights Reserved.
## 
## $Id: dist.sh,v 1.1 2004/01/10 22:29:18 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

VERSION=$1
if test x$VERSION = x ; then
    echo the first and only argument should be the version number of the release.
    exit 1
fi
TARBALL=cplusql-${VERSION}
rm -rf ${TARBALL}.tar.gz
rm -rf ${TARBALL}.tar.gz.md5
rm -rf $TARBALL
mkdir -p $TARBALL
mkdir -p $TARBALL/inc
mkdir -p $TARBALL/src
mkdir -p $TARBALL/test

for FILE in $( find . ! -path "*CVS*" -a ! -path "cplusql" ) ; do
    cp $FILE $TARBALL/$FILE
done

tar czvf ${TARBALL}.tar.gz ${TARBALL}/
rm -rf ${TARBALL}
md5sum ${TARBALL}.tar.gz >${TARBALL}.tar.gz.md5
