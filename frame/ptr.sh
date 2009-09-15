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
## $Id: ptr.sh,v 1.1 2004/01/10 21:19:05 mgrosso Exp $
## ----------------------------------------------------------------------
## ----------------------------------------------------------------------

VERSION=$1
TARBALL=ptr-${VERSION}
mkdir ${TARBALL}
mkdir ${TARBALL}/inc/
mkdir ${TARBALL}/src/
mkdir ${TARBALL}/test/
cp    inc/ptr.h  ${TARBALL}/inc/
cp    inc/PtrSemaphore.h  ${TARBALL}/inc/
cp    inc/PtrSemaphore.i  ${TARBALL}/inc/
cp    inc/Semaphore.h  ${TARBALL}/inc/
cp    src/Semaphore.cpp  ${TARBALL}/src
cp    test/test-ptr.cpp  ${TARBALL}/test
cp    test/test-ptrsemaphore.cpp  ${TARBALL}/test
tar czvf ${TARBALL}.tar.gz ${TARBALL}/
rm -rf ${TARBALL}
md5sum ${TARBALL}.tar.gz >${TARBALL}.tar.gz.md5
