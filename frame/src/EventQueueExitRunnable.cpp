#ident "file_id $Id: EventQueueExitRunnable.cpp,v 1.1 2004/02/28 02:56:31 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      EventQueueExitRunnable.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Fri Feb 20 17:57:45 EST 2004 on localhost.localdomain
// Project:   
// Purpose:   
// 
// $Id: EventQueueExitRunnable.cpp,v 1.1 2004/02/28 02:56:31 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "EventQueueExitRunnable.h"
#include <stdlib.h>

EventQueueExitRunnable::EventQueueExitRunnable( EventQueue *eq )
:eq_(eq)
{
    ;//noop
}

EventQueueExitRunnable::~EventQueueExitRunnable()
{
    ;//noop
}

void
EventQueueExitRunnable::run()
{
    eq_->wait();
    exit(0);
}

