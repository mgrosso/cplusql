
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      Daemon.h
// Author:    mgrosso Matt Grosso
// Created:   Fri Mar 26 12:47:20 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// ----------------------------------------------------------------------
// to play nice as a daemon, we do 3 things.
// 1- fork() early, so parent doesnt accidentally wait() up worrying.
// 2- chdir( somewhere ), so that sysadmins can unmount our current filesystem 
//    without kill()ing us later on
// 3- close default filehandles, which we may be a tty, which we dont need.
// ----------------------------------------------------------------------
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: Daemon.h,v 1.1 2004/03/26 22:46:09 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#ifndef DAEMON_H
#define DAEMON_H 1


class Daemon
{
public:
    static void daemonize(  
        bool dofork, const char *homedir, const char *pidfile );
};

#endif /* DAEMON_H */
