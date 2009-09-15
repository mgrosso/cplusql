
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      cplusql.h
// Author:    mgrosso 
// Created:   Sun Sep 25 21:55:45 PDT 2005 on caliban
// Project:   
// Purpose:   
// 
// $Id: cplusql.h,v 1.41 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef CPLUSQL_H
#define CPLUSQL_H 1

#ifdef PACKAGE
#define CLOBBERED_PACKAGE PACKAGE
#undef PACKAGE
#endif /* PACKAGE */

#include "cplusql_config.h"

#ifdef PACKAGE
#define CPLUSQL_PACKAGE PACKAGE
#undef PACKAGE
#ifdef CLOBBERED_PACKAGE 
#define PACKAGE CLOBBERED_PACKAGE
#undef CLOBBERED_PACKAGE 
#endif  /* CLOBBERED_PACKAGE */
#endif /* PACKAGE */

#include "cplusql_api.h"
#endif /* CPLUSQL_H */
