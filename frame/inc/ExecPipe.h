// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      ExecPipe.h
// Author:    matt Matthew Grosso
// Created:   Fri Nov 26 22:11:37 EST 1999 on circe
// Project:   frame
// Purpose:   provides easy way to exec a process and pipe to/from it.
// 
// Copyright (c) 1999 Primary Knowledge Inc.  All Rights Reserved.
// 
// $Id: ExecPipe.h,v 1.2 2004/02/12 21:16:45 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef EXECPIPE_H
#define EXECPIPE_H 1

#include "Dest.h"
#include "ByteSource.h"


class ExecPipePriv ;

class ExecPipe {
    public:

    ExecPipe( const char *exec, bool pipein, bool pipeout, bool pipeerr );
    ~ExecPipe();

    pDest &             getStdInput();
    ByteSourcePtr &     getStdOutput();
    ByteSourcePtr &     getStdError();

    bool                isDone();
    pid_t               getPid();
    int                 getResult();

    private:
    ExecPipePriv *d_;

    //NOT IMPLEMENTED
    ExecPipe();
    ExecPipe( const ExecPipe & );
    const ExecPipe &operator=( const ExecPipe & );
};

#endif /* EXECPIPE_H */
