// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SharedServiceMain.h
// Author:    mgrosso Matt Grosso
// Created:   Sat Feb 28 11:16:21 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: SharedServiceMain.h,v 1.19 2004/08/20 21:56:17 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef SHAREDSERVICEMAIN_H
#define SHAREDSERVICEMAIN_H 1


//this is slightly more includes than strictly necessary for this interface,
//but that is in keeping with the spirit of this convenience wrapper class,
//which is to make programming cplusql CORBA Service and Client apps as 
//easy as possible.

#include "Runnable.h"
#include "AsyncRunnable.h"
#include "AppLog.h"
#include "AppConfig.h"
#include "AppExceptionHandler.h"

// tao/ and orbsvcs/ should be in -I inc/ actually ...
#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

#include "serviceC.h"
#include "clusterioC.h"
#include "NVPairUtil.h"
#include "NodeCntl.h"

class SharedServiceMainPriv ;

class SharedServiceMain 
    //: public AsyncRunnable
    : public Runnable
{
public:
    static void                 init( int argc, char **argv, const char *name, bool server=true );
    static SharedServiceMain *  instance();
	static void                 shutdown(bool hard = true);

    static int                  serviceMain(
                                    int argv, 
                                    char **argv,
                                    const char *name, 
                                    PortableServer::Servant (*factory)(),
                                    bool gs_name = false,
                                    bool register_nms = false,
                                    const char *iorfilekey = NULL, 
                                    const char *subscribe_service = NULL
                                    );

    static CORBA::Object_ptr    genericClientMain();

    CORBA::ORB_var              getORB();
    PortableServer::POA_var     getPOA();
    CORBA::Object_ptr           resolve( const char *url );
    bool                        resolve_service( 
                                    Service::GenericService_var &service, 
                                    const char *who );
    bool                        resolve_consumer( 
                                    Service::NodeMonitorPushConsumer_var &nodeMonitorPushConsumer,
                                    const char *who );
    bool                        resolve_helper( 
                                    CORBA::Object_ptr &obj, 
                                    const char *url );

    void                        writeIOR( 
                                    const char *ior_file_key, 
                                    PortableServer::Servant p_servant );

    void                        activate( 
                                    PortableServer::Servant p_servant
                                );

    void                        reconfigure( 
                                    const Service::NVPairList &config, Config *c );

    void                        registerSelf();
    virtual void                start();
    virtual void                run();
    virtual void                wait();//blocks until someone else calls shutdown

    Service::ServiceStatus *    makeStatus( 
                                    bool shutdownPending,    
                                    Config &state );

    char_ptr                    makeLocalUrl( const char *service );
    char_ptr                    makeUrl( const char *corbaname );
    const char *                myHostname();

    void                        initServiceRef( Service::ServiceRef &s );

    static string               pullStatusClient( 
                                clusterio::NameSpaceService_ptr &service);
                                    //Service::GenericService_ptr &service);

    void                        subscribe(
                                    const char *service, const char *host);
    static NodeCntlPtr                 makeNodeCntl();
protected:
    SharedServiceMainPriv *     d_ ;
    static SharedServiceMain *  instance_ ;

    SharedServiceMain( int argc, char **argv, const char *name, bool server=true );
	virtual ~SharedServiceMain();

private:
//not implemented
    SharedServiceMain();
    SharedServiceMain(const SharedServiceMain &);
    SharedServiceMain & operator=(const SharedServiceMain &);
};

#endif /* SHAREDSERVICEMAIN_H */
