// -*- C++ -*-
//
// $Id: GenericServiceImpl.cpp,v 1.8 2004/03/25 17:45:07 holahrei Exp $

// ****  Code generated by the The ACE ORB (TAO) IDL Compiler ****
// TAO and the TAO IDL Compiler have been developed by:
//       Center for Distributed Object Computing
//       Washington University
//       St. Louis, MO
//       USA
//       http://www.cs.wustl.edu/~schmidt/doc-center.html
// and
//       Distributed Object Computing Laboratory
//       University of California at Irvine
//       Irvine, CA
//       USA
//       http://doc.ece.uci.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from 
// be/be_codegen.cpp:985

#include "GenericServiceImpl.h"
#include <stdio.h>
#include <unistd.h>
#define DEBUG = 1
using namespace std;
#include <iostream>
#include "Exclusion.h"
#include "Thread.h"

class exmain {
	public:
		Mutex m;
		Condition c;
};

exmain *ex = new exmain();
int pingCount=0;

frame_thread_return_t
func1( void *x ){
    exmain *ex = (exmain *)x;
    while(1) {
        {
            cerr << "in thread: "<<endl;
            MutexGuard mg( ex->m );
            int pc=pingCount;
            cerr << "  pingCount="<<pc<<endl;
            sleep( 1 );
            pingCount=pc+1;
        }
        sleep( 10 );
    }
};

// Implementation skeleton constructor
Service_GenericServiceImpl::Service_GenericServiceImpl (void)
  {
    cerr << "creating new Thread"<<endl;
	Thread t( func1, ex );
  }
  
// Implementation skeleton destructor
Service_GenericServiceImpl::~Service_GenericServiceImpl (void)
  {
  }
  
void Service_GenericServiceImpl::ping (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ))
  {
    cerr<<"in ping:"<<endl;
    MutexGuard mg( ex->m );
    int pc=pingCount;
    cerr << "  pingCount="<<pc<<endl;
    sleep( 2 );
    pingCount=pc+1;
  }
  
::Service::ServiceStatus * Service_GenericServiceImpl::pullStatus (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
  ))
  {
    // Add your implementation here
  }
  
void Service_GenericServiceImpl::reconfigure (
    const Service::NVPairList & configuration
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ))
  {
    // Add your implementation here
  }
  
void Service_GenericServiceImpl::gracefulShutdown (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ))
  {
    // Add your implementation here
  }
  
