// -*- C++ -*-
//
// $Id: JobExecServiceImpl.h,v 1.5 2004/03/24 23:25:15 mgrosso Exp $

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
// and
//       Institute for Software Integrated Systems
//       Vanderbilt University
//       Nashville, TN
//       USA
//       http://www.isis.vanderbilt.edu/
//
// Information about TAO is available at:
//     http://www.cs.wustl.edu/~schmidt/TAO.html

// TAO_IDL - Generated from 
// be/be_codegen.cpp:969

#ifndef JOBEXECSERVICEI_H_
#define JOBEXECSERVICEI_H_

#include "JobExecServiceS.h"
#include "Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// TAO_IDL - Generated from
// be/be_visitor_interface/interface_ih.cpp:57
class  JobExecServicePriv ;

class  JobExec_JobExecService_i : public virtual POA_JobExec::JobExecService
{
public:
  //Constructor 
  JobExec_JobExecService_i ( const Config &c );
  
  //Destructor 
  virtual ~JobExec_JobExecService_i (void);
  
  // TAO_IDL - Generated from
  // be/be_visitor_operation/operation_ih.cpp:43
  
  virtual CORBA::Boolean runJob (
      const JobQueue::JobQueueRef & jobQueueRef,
      const JobQueue::JobSubmissionStruct & jobSubmission
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
      , Service::ServiceException
      , Service::PendingShutdownException
    ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual void cleanupStaleOutput (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual void killAllRunning (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual void ping (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual ::Service::ServiceStatus * pullStatus (
    
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual void reconfigure (
    const Service::NVPairList & configuration
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

virtual void gracefulShutdown (
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
    , Service::ServiceException
    , Service::PendingShutdownException
  ));// TAO_IDL - Generated from
// be/be_visitor_operation/operation_ih.cpp:43

private:
    JobExecServicePriv *d_ ;

    //not implemented
    JobExec_JobExecService_i ();
    JobExec_JobExecService_i (const JobExec_JobExecService_i & rhs );
    JobExec_JobExecService_i & operator =(const  JobExec_JobExecService_i & rhs );
};


#endif /* JOBEXECSERVICEI_H_  */
