#ident "SharedServiceMain.cpp $Id: SharedServiceMain.cpp,v 1.41 2004/08/30 21:30:40 holahrei Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      SharedServiceMain.cpp
// Author:    mgrosso Matt Grosso
// Created:   Sat Feb 28 11:45:19 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
//
// Copyright (c) 2004 LookSmart. All Rights Reserved.
//
// $Id: SharedServiceMain.cpp,v 1.41 2004/08/30 21:30:40 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "ptr_strdup.h"
#include "AppLog.h"
#include "AppConfig.h"
#include "ConfigDump.h"
#include "ThreadRunnable.h"
#include "ProxyRunnable.h"
#include "Daemon.h"
#include "DestFile.h"
#include "Exclusion.h"
#include "Slurp.h"
#include "ace/Object_Manager.h"

#include "keys.h"
#include "NVPairUtil.h"
#include "SharedServiceMain.h"

#include <sys/utsname.h>

SharedServiceMain * SharedServiceMain::instance_ = NULL ;  

class SharedServiceMainPriv 
{
    friend class SharedServiceMain;  
    CORBA::ORB_var                  orb_;
    PortableServer::POA_var         poa_;
    PortableServer::POAManager_var  poa_manager_ ;
    CosNaming::NamingContext_var    naming_context_ ;
    AsyncRunnablePtr                tr_;
    char_ptr                        ior_ ;
};

void                 
SharedServiceMain::init( int argc, char **argv, const char *name, bool server )
{
    if(!instance_)
    {
        instance_=new SharedServiceMain( argc, argv, name, server );
    }
}

SharedServiceMain::SharedServiceMain( 
    int argc, char **argv, const char *name, bool server )
{
    static int namebuflen = 1024 ;
    char namebuf [ namebuflen + 1] ;
    namebuf[ namebuflen ] = '\0' ;

    //initialize configuration singleton.
    AppConfig::instance()->init( argc, argv );
    //by default, APPLOG_DBG messages are not printed
    //APPLOG_INFO, and more serious messages are printed.
    //APPLOG_DBG messages can be printed by setting APPLOG_MINLOG=0
    //in the environment or command line
    AppConfig::instance()->addString(   "APPLOG_MINLOG", "1", 0 );

    if( !server )
    {
        APPLOG_DBG("ctor() starting ... ");
        d_=new SharedServiceMainPriv();
        APPLOG_DBG("ctor() calling orb init... ");
        d_->orb_ = CORBA::ORB_init( argc, argv, "SharedServiceMain_orb_" );
        APPLOG_DBG("ctor() initialization complete ");
        return ;
    }

    //these must exist, or else we throw.
    const char *nshost = AppConfig::instance()->getString( NS_HOST );
    const char *nsport = AppConfig::instance()->getString( NS_PORT );
    //CD TO CORRECT DIREctory before logging begins.
    const char *topdir = AppConfig::instance()->getString(   GS_ROOTDIR, "."  );
    if( 0 != chdir( topdir ))
    {
        PANICV("cant chdir %s", topdir );
    }

    //initialize logging singleton.
    //by default we want to log everything to stderr, and log only 
    //alarm and abort to the syslog, with the identity of name
    AppConfig::instance()->addString(   "APPLOG_SYSLOG_IDENT", name, 0 );
    AppConfig::instance()->addString(   "APPLOG_MINSYSLOG", "3", 0 );

    const char *instance_id = AppConfig::instance()->getString( GS_INSTANCE_ID, "0" );
    const char *servicename = AppConfig::instance()->getString( GS_SERVICENAME, name );

    const char * myhostname = myHostname();
    snprintf( namebuf, namebuflen, "%s_%s_%s", myhostname, instance_id, servicename);

    AppConfig::instance()->addString(GS_CORBANAME, namebuf, false);

    char_ptr logfilename = ptr_strdup_plus(
        AppConfig::instance()->getString(GS_CORBANAME ), ".log" );

    const char *logfile = AppConfig::instance()->getString(APPLOG_DEST_CFKEY, logfilename.get());

    fprintf( stderr, "logging to %s\n", logfile );
    AppLog::instance()->reconfigure(*AppConfig::instance());
    AppLog::instance()->useFile(logfile );

    //log at least something before doing orb init stuff.
    //no other debug logging done here, since SharedServiceMain has it allready.
    APPLOG_INFO("ctor() starting ... ");
    d_=new SharedServiceMainPriv();
    APPLOG_DBG("ctor() calling orb init... ");
    d_->orb_ = CORBA::ORB_init( argc, argv, "SharedServiceMain_orb_" );

    bool dofork = AppConfig::instance()->getBool( GS_FORK, "yes" );
    const char *rootdir  = AppConfig::instance()->getString( GS_ROOTDIR, "" );
    char_ptr dflt_pidfile = ptr_strdup_plus( 
        AppConfig::instance()->getString( GS_CORBANAME ), ".pid" );

    const char *pidfile  = AppConfig::instance()->getString( 
        GS_PIDFILE, dflt_pidfile.get() );

    APPLOG_DBG("ctor() calling Daemon::daemonize( %i, %s, %s ) ",
        dofork, rootdir, pidfile );
    Daemon::daemonize( dofork, rootdir, pidfile );


    APPLOG_DBG("ctor() getting poa object ... ");
    CORBA::Object_var poa_object = 
        d_->orb_->resolve_initial_references( "RootPOA" );

    APPLOG_DBG("ctor() narrowing poa ... ");
    d_->poa_ = PortableServer::POA::_narrow (poa_object.in ());

    APPLOG_DBG("ctor() gettin poa manager ... ");
    PortableServer::POAManager_var poa_manager =  d_->poa_->the_POAManager (); 

    APPLOG_DBG("ctor() activating poa ... ");
    poa_manager->activate();

    snprintf(namebuf, namebuflen, "iiop://%s:%s/NameService",
        nshost, nsport);

    APPLOG_DBG("ctor() retrieving nameservice object, %s ... ", namebuf);
    CORBA::Object_var naming_context_object =
        d_->orb_->string_to_object (namebuf );

    if( ! naming_context_object )
    {
        PANICV( "cant string_to_object nameservice ref: %s", namebuf );
    }

    APPLOG_DBG("ctor() narrowing nameservice object, %x ... ", naming_context_object.in ());
    d_->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_context_object.in ());

    if( ! d_->naming_context_ )
    {
        PANICV( "cant narrow nameservice obj: %s", namebuf );
    }

    snprintf(namebuf, namebuflen, "corbaname::%s:%s#%s",
        nshost, nsport, AppConfig::instance()->getString(GS_CORBANAME));
    const char *my_url = AppConfig::instance()->getString( GS_MY_URL, namebuf );
    APPLOG_INFO("ctor() my url is %s", my_url );

    try{
        APPLOG_INFO("ctor() find eviltwin: string_to_object %s", my_url );
        CORBA::Object_var eviltwin_obj =
            d_->orb_->string_to_object ( my_url );
        if( eviltwin_obj )
        {
            APPLOG_INFO("ctor() find eviltwin: narrowing object at %s", my_url );
            Service::GenericService_ptr eviltwin_ptr = 
                Service::GenericService::_narrow( eviltwin_obj.in());
            if( eviltwin_ptr )
            {
                APPLOG_INFO("ctor() find eviltwin: ping() object at %s", my_url );
                eviltwin_ptr->ping();
                APPLOG_INFO("ctor() exit(0) because found an eviltwin at %s", my_url );
                exit(0);
            }
        }
    }catch( ... )
    { 
        APPLOG_INFO("ctor() good: no eviltwin at %s", my_url );
    }

    snprintf(namebuf, namebuflen, "corbaname::%s:%s#%s_0_NodeMonitorService",
        nshost, nsport, AppConfig::instance()->getString(GS_HOSTNAME ));

    AppConfig::instance()->addString( GS_DEFAULT_REGISTER_TARGET_URL, namebuf, false );

    APPLOG_INFO("ctor() initialization complete ");
}


SharedServiceMain::~SharedServiceMain()
{
	if (d_ != (SharedServiceMainPriv *) 0)
	{
		if (d_->naming_context_ != (CosNaming::NamingContext_var) 0)
		{
			d_->naming_context_ = (CosNaming::NamingContext_var) 0;
			APPLOG_INFO("naming_context gone");
		}

		if (d_->poa_manager_ != (PortableServer::POAManager_var) 0)
		{
			d_->poa_manager_ = (PortableServer::POAManager_var) 0;
			APPLOG_INFO("poa_manager gone");
		}

		if (d_->poa_ != (PortableServer::POA_var) 0)
		{
	    	d_->poa_->destroy(1,1);
			d_->poa_ = (PortableServer::POA_var) 0;
			APPLOG_INFO("poa destroy(1,1)");
		}

		if (d_->orb_ != (CORBA::ORB_var) 0)
		{
			d_->orb_->destroy();
			d_->orb_ = (CORBA::ORB_var) 0;
			APPLOG_INFO("orb->destroy()-ed");
		}

		delete d_;
		d_ = (SharedServiceMainPriv *) 0;
	}

}


void
SharedServiceMain::activate( PortableServer::Servant p_servant)
{

    APPLOG_DBG("activate() activating servant object ... ");
    d_->poa_->activate_object( p_servant );

    APPLOG_DBG("activate() getting object reference to servant... ");
    CORBA::Object_ptr obj = d_->poa_->servant_to_reference( p_servant );

    const char *namebuf = AppConfig::instance()->getString(GS_CORBANAME);
        
    CosNaming::Name corbaname (1);
    corbaname.length (1);
    corbaname[0].id = CORBA::string_dup ( namebuf );

    APPLOG_DBG("activate() rebind() obj reference to name %s ... ", 
        namebuf );
    d_->naming_context_->rebind (corbaname, obj );
    APPLOG_DBG("activate() saving ior ... " );
    d_->ior_ = ptr_strdup( d_->orb_->object_to_string( obj ));
    APPLOG_DBG("activate() ior is %s", d_->ior_.get());
}


SharedServiceMain *  
SharedServiceMain::instance()
{
    if( ! instance_ )
    {
        PANICV("instance() called prior to init()");
    }
    return instance_;
}


void
SharedServiceMain::shutdown(bool hard)
{
	static Mutex mtx;
	MutexGuard guard(mtx);
	bool caughtSomething = false;

	if (instance_ != (SharedServiceMain *) 0)
	{
		if (instance_->d_ != (SharedServiceMainPriv *) 0)
		{
			if (instance_->d_->orb_ != (CORBA::ORB_var) 0)
			{
				try {
					bool block = !hard;
					instance_->d_->orb_->shutdown(block);
					APPLOG_INFO("orb->shutdown()");
				} catch (...) {
					caughtSomething = true;
					APPLOG_WARN("Failed to shutdown orb");
				}

				if (hard)
				{
					try {
						instance_->d_->orb_->destroy();
						instance_->d_->orb_ = (CORBA::ORB_var) 0;
						APPLOG_INFO("orb->destroyed");
					} catch (...) {
						caughtSomething = true;
						APPLOG_WARN("Failed to destroy orb");
					}
				}
			}
		}

		if (!caughtSomething || hard)
		{
			if (instance_ != (SharedServiceMain *) 0)
			{
				delete instance_;
				instance_ = (SharedServiceMain *) 0;
			}
		}
	}

	if (hard)
	{
		if (!ACE_Object_Manager::shutting_down())
		{
			APPLOG_INFO("deleting ACE_Object_Manager");

			//@@TODO: is this safe?  ACE_Object_Manager_Manager may do this.
			//
			delete ACE_Object_Manager::instance();
		}
	
		ACE_Init_ACE::fini();

		AppExceptionHandler::cleanup();
		AppConfig::shutdown();
		AppLog::shutdown();
	}
}


CORBA::ORB_var
SharedServiceMain::getORB()
{
    return d_->orb_ ;
}

PortableServer::POA_var
SharedServiceMain::getPOA()
{
    return d_->poa_ ;
}

void
SharedServiceMain::writeIOR( const char *ior_file_key, 
                            PortableServer::Servant p_servant )
{
    if( ! ior_file_key )
    {
        APPLOG_INFO("not writing ior, NULL config key" );
        return;
    }
    const char *gs_name = AppConfig::instance()->getString(GS_CORBANAME);
    char_ptr dfltiorfile = ptr_strdup_plus( gs_name, ".ior" );
    const char *iorfile =AppConfig::instance()->getString(
        ior_file_key, dfltiorfile.get());
    if( !iorfile )
    {
        APPLOG_INFO (
            "not writing ior, NULL config value for %s", ior_file_key );
    }
    APPLOG_DBG ("writing ior to %s", iorfile );
    CORBA::Object_ptr obj = d_->poa_->servant_to_reference( p_servant );
    APPLOG_DBG ("done servant_to_reference, now open iorfile...");
    DestFile iordest( iorfile );
    APPLOG_DBG ("get ior and write to file ...");
    const char *objstr = d_->orb_->object_to_string( obj );
    iordest.put( objstr );
    APPLOG_INFO ("wrote ior [%s] to file %s", objstr, iorfile );
}

CORBA::Object_ptr
SharedServiceMain::resolve( const char *url )
{
    APPLOG_DBG ("resolve(%s)", url );
    return d_->orb_->string_to_object( url );
}

bool
SharedServiceMain::resolve_helper( CORBA::Object_ptr &obj, const char *url )
{
    if( NULL == url || '\0' == *url )
    {
        APPLOG_DBG ("resolve helper null arg");
        return false; 
    }
    APPLOG_DBG ("resolve(%s)", url );
    obj = d_->orb_->string_to_object( url );
    return true;
}

bool 
SharedServiceMain::resolve_consumer( Service::NodeMonitorPushConsumer_var &nodeMonitorPushConsumer, const char *who )
{
    try{
        CORBA::Object_ptr obj ;
        APPLOG_DBG("resolving string %s", who );
        if( ! this->resolve_helper( obj, who ))
        {
            return false;
        }
        APPLOG_DBG("resolved string %s, narrowing to nm push consumer...", who );
        nodeMonitorPushConsumer = Service::NodeMonitorPushConsumer::_narrow (obj);
        if (CORBA::is_nil (nodeMonitorPushConsumer.in ()))
        {
            APPLOG_DBG("string %s could not be narrowed", who );
            return false;
        }
        APPLOG_DBG("string %s narrowed ok", who );
        return true;
    }
    catch( CORBA::Exception &ce) 
    {
        APPLOG_ABORT("caught CORBA::Exception trying to resolve or narrow %s [%s][%s]", 
            who, ce._rep_id(), ce._name() );
    }
    return false;
};

bool 
SharedServiceMain::resolve_service( Service::GenericService_var &service, const char *who )
{
    CORBA::Object_ptr obj ;

    APPLOG_DBG("resolving string %s", who );
    if( ! resolve_helper( obj, who ))
    {
        return false;
    }
    APPLOG_DBG("resolved string %s, narrowing to nm push consumer...", who );
    service = Service::GenericService::_narrow (obj);
    if (CORBA::is_nil (service.in ()))
    {
        APPLOG_DBG("string %s could not be narrowed", who );
        return false;
    }
    APPLOG_DBG("string %s narrowed ok", who );
    return true;
};


void
SharedServiceMain::start()
{
    RunnablePtr rp = new ProxyRunnable( this);
    d_->tr_ = new ThreadRunnable( rp );
    d_->tr_->start();
}

void
SharedServiceMain::run()
{
    APPLOG_INFO("run() entering orb->run()");
    d_->orb_->run();
    APPLOG_INFO("run() completed orb->run()");
}

void
SharedServiceMain::wait()
{
    d_->tr_->wait();
}

Service::ServiceStatus *    
SharedServiceMain::makeStatus( bool shutdownPending, Config &state )
{
    APPLOG_DBG("makeStatus()");

    Service::ServiceStatus * ret = new Service::ServiceStatus();

    ret->ref.serviceName = AppConfig::instance()->getString(GS_SERVICENAME);
    ret->ref.url = AppConfig::instance()->getString( GS_MY_URL );
    ret->ref.hostname = AppConfig::instance()->getString( GS_HOSTNAME );
    ret->ref.ior = const_cast< const char *> (d_->ior_.get()) ;//const forces copy

    ret->statusReceivedTime=0;
    ret->pid=getpid();
    ret->isPendingShutdown=shutdownPending;
    ret->live=true;
    Service::NVPairList * nvpl=NVPairUtil::convert( state );
    ret->details = *nvpl;
    delete nvpl;

    string logstatus ;
    ConfigDump::dumpToString( state, logstatus );

    APPLOG_INFO(
        "makeStatus() url=%s svc=%s host=%s pid=%d shutdown=%d cfg(%s)",
        ret->ref.url.in(),
        ret->ref.serviceName.in(),
        ret->ref.hostname.in(),
        ret->pid=getpid(),
        ret->isPendingShutdown=shutdownPending,
        logstatus.c_str()
    );
    return ret;
}

void
SharedServiceMain::registerSelf()
{
    // This must be called by any service (on only be a service) which
    // needs to register itself on its NodeMonitorService
    const char* defaultRegisterTargetURL = AppConfig::instance()->getString( GS_DEFAULT_REGISTER_TARGET_URL );
    
    Service::ServiceRef ref;
    initServiceRef( ref );

    APPLOG_INFO("registering self (%s %s %s) on %s", 
        ref.serviceName.in(),
        ref.url.in(),
        ref.hostname.in(),
        defaultRegisterTargetURL
        );

    APPLOG_DBG("registerSelf(): %s calling resolv()", defaultRegisterTargetURL);
    
    CORBA::Object_ptr obj = resolve( defaultRegisterTargetURL );

    APPLOG_DBG("registerSelf(): %s calling _narrow()", defaultRegisterTargetURL);
    // Now downcast the object reference to the appropriate type
    Service::NodeMonitorService_var nodeMonitorService =
      Service::NodeMonitorService::_narrow (obj);

    if (CORBA::is_nil (nodeMonitorService.in ()))
    {
        PANICV("ERROR: nodeMonitorService.in () object reference is_nil for %s", defaultRegisterTargetURL);
        return; // not reached
    }

    nodeMonitorService->registerService(ref);
    APPLOG_DBG("registerSelf() done.");
}

char_ptr
SharedServiceMain::makeUrl( const char *corbaname )
{
    char urlbuf[ 1024 ];
    snprintf( urlbuf, 1023, "corbaname::%s:%s#%s",
        AppConfig::instance()->getString( NS_HOST ),
        AppConfig::instance()->getString( NS_PORT ),
        corbaname );
    return ptr_strdup( urlbuf );
}

char_ptr
SharedServiceMain::makeLocalUrl( const char *service )
{
    char corbaname[ 1024 ];
    snprintf( corbaname, 1023, "%s_0_%s", myHostname(), service);
    return makeUrl( corbaname );
}

const char *
SharedServiceMain::myHostname()
{
    //we use uname for default hostname, and allow user to override.
    struct utsname u ;
    (void)uname ( &u );
    char *period = strchr( u.nodename, '.' );
    if( period && *period )
    {
        *period = '\0';
    }
    return AppConfig::instance()->getString(GS_HOSTNAME, u.nodename );
}

void                        
SharedServiceMain::initServiceRef( Service::ServiceRef &ref )
{
    ref.serviceName = AppConfig::instance()->getString(GS_SERVICENAME);
    ref.url = AppConfig::instance()->getString( GS_MY_URL );
    ref.hostname = myHostname();
    ref.ior = const_cast< const char *> (d_->ior_.get()) ;//const forces copy
}

void                        
SharedServiceMain::reconfigure( const Service::NVPairList & changes, 
    Config *existing )
{
    Service::ServiceException err;
    try{
        if( ! existing )
        {
            PANICV( "Null pointer for existing configuration");
        }
        ConfigPtr newc = NVPairUtil::convert( changes );
        APPLOG_INFO( "reconfigure()ing ...");
        existing->slurp( newc, true );
        AppLog::instance()->reconfigure( *existing );
        string newconfig ;
        ConfigDump::dumpToString( *newc, newconfig );
        APPLOG_INFO( "reconfigure()ed with: %s", newconfig.c_str());
        return;
    }
    catch(exception &e )
    {
        APPLOG_WARN( "reconfigure() caught execption: %s", e.what());
        err.what_string = e.what();
    }
    catch( ... )
    {
        APPLOG_WARN( "reconfigure() caught unknown exception");
        err.what_string = "reconfigure() caught unknown exception";
    }
    throw err;
}

void
SharedServiceMain::subscribe( const char *service, const char *host )
{
    char_ptr url_default = makeUrl( CNMS_NAME );
    const char* cnms_url = AppConfig::instance()->getString(
        GS_DEFAULT_SUBSCRIBE_TARGET_URL, url_default.get()); 
    CORBA::Object_ptr obj = resolve(cnms_url);
    if( ! obj )
    {
        PANICV("could not resolve to object: %s", cnms_url );
    }
    APPLOG_DBG( "resolved string to object, now _narrow... ");
    Service::NodeMonitorService_ptr nms =
          Service::NodeMonitorService::_narrow (obj);
    if ( ! nms )
    {
        PANICV("object reference is_nil for %s", cnms_url);
    }
    APPLOG_DBG("_narrow ok");
    Service::ServiceRef sref;
    initServiceRef( sref );
    APPLOG_DBG("subscribing ...");
    nms->subscribePushConsumer( sref, service, host );
    APPLOG_DBG("subscribe done");
}

int
SharedServiceMain::serviceMain( 
    int argc,
    char **argv,
    const char *name,
    PortableServer::Servant (*factory)(),
    bool gs_name,
    bool register_nms,
    const char *iorfilekey,
    const char *subscribe_service 
    )
{
	int retVal = 4;

    try {
        if(!name)
        {
            return 13;
        }
        if( gs_name )
        {
            AppConfig::instance()->addString(GS_CORBANAME, NSS_NAME, true);
        }
        //initialize singletons: AppConfig, AppLog, orb, root poa 
        SharedServiceMain::init( argc, argv, name );

        //create CORBA servant 
        PortableServer::Servant servant = factory();

        SharedServiceMain *ssm = SharedServiceMain::instance();

        //activate CORBA servant, and register with name service 
        ssm->activate(servant);

        //register self with local node monitor
        if( register_nms)
        {
            ssm->registerSelf();
        }
        //write ior file
        if( iorfilekey )
        {
            ssm->writeIOR( iorfilekey, servant);
        }
        //subscribe to central node monitor
        if( subscribe_service )
        {
            ssm->subscribe( subscribe_service, "*" );
        }

        //main loop
        SharedServiceMain::instance()->run();
    }
    catch (Service::ServiceException &se) 
    {
        APPLOG_ABORT( "%s main() caught SystemException[%s][%s][%s]", 
            name, se._rep_id(), se._name(), se.what_string.in());
    }
    catch (CORBA::Exception &ce) 
    {
        APPLOG_ABORT("%s main() caught CORBA::Exception [%s][%s]", 
            name, ce._rep_id(), ce._name() );
		retVal = 1;
    }
    catch (std::exception &e) 
    {
        APPLOG_ABORT("%s main() caught std:exception [%s]", 
            name, e.what());
		retVal = 2;
    }
    catch ( ... ) 
    {
        APPLOG_ABORT("%s main() caught unknown exception [%s]", name );
		retVal = 3;
    }

	// We won't catch anything here, since AppLogging is possible not
	// working.  And by not catching, we'll leave open the ability to dump
	// core, so we can see why shutdown failed.
	//
	SharedServiceMain::shutdown();

    return retVal;
}

string
SharedServiceMain::pullStatusClient(clusterio::NameSpaceService_ptr &service) 
{
    APPLOG_DBG( "pullStatusClient() calling pullStatus()");
    ::Service::ServiceStatus *stat= service->pullStatus();
    if( ! stat )
    {
        PANICV("pullStatusClient()  returned NULL!");
    }
    APPLOG_INFO(
"pullStatusClient() name=%s url=%s host=%s pid=%lu shutdown=%i live=%i ior=%s" ,
        stat->ref.serviceName.in(),
        stat->ref.url.in(),
        stat->ref.hostname.in(),
        static_cast< unsigned long > (stat->pid),
        stat->isPendingShutdown,
        stat->live,
        stat->ref.ior.in()
    );
    string cfgstr ;
    ConfigPtr c=NVPairUtil::convert( stat->details );
    ConfigDump::dumpToString( *c, cfgstr );
    APPLOG_INFO("pullStatus()  ConfigDump: %s", cfgstr.c_str() );
    return cfgstr ;
}

CORBA::Object_ptr
SharedServiceMain::genericClientMain()
{
    APPLOG_DBG("genericClientMain()" );
    const char * objstr = AppConfig::instance()->findString( "GS_IOR_URI" );
    if( ! objstr )
    {
        const char *filename = 
            AppConfig::instance()->findString( "GS_IOR_FILE" );
        if( filename )
        {
            objstr = Slurp::slurpFile( filename );
        }
    }
    if( !objstr )
    {
        objstr = AppConfig::instance()->getString( "GS_URL" );
    }
    bool status = AppConfig::instance()->getBool( "DOSTATUS", "0" );
    const char *reconfigure = 
        AppConfig::instance()->findString( "RECONFIGURE");
    bool shutdown = AppConfig::instance()->getBool( "DOSHUTDOWN", "0" );

    APPLOG_DBG("objstr[%s]", objstr );

    CORBA::Object_ptr obj = 
        SharedServiceMain::instance()->resolve( objstr );
    if( ! obj )
    {
        PANICV( "could not resolv string to object: %s.", objstr  );
    }
    APPLOG_DBG("resolved, now _narrow() to GenericService ... ");
    clusterio::NameSpaceService_ptr gs = clusterio::NameSpaceService::_narrow( obj );
    if( !gs )
    {
        PANICV("GenericService::_narrow()  returned NULL!");
    }

    //The following statement might cause problems when porting
    //this code to a different orb, other than TAO.
    APPLOG_DBG("resolved and narrowed to GenericService, repo id=%s",
        gs->_interface_repository_id()
    );

    APPLOG_DBG("doing ping()...");
    gs->ping();
    APPLOG_DBG("done  ping()");

    if( status)
    {
        (void)SharedServiceMain::pullStatusClient( gs );
    }
    if( reconfigure )
    {
        Config c(reconfigure);
        Service::NVPairList *nvp = NVPairUtil::convert( c );
        APPLOG_DBG("running reconfigure(%s)", reconfigure );
        gs->reconfigure( *nvp );
        APPLOG_DBG("done with reconfigure()" );
    }
    if( shutdown )
    {
        APPLOG_DBG("running gracefulShutdown()" );
        gs->gracefulShutdown();
        APPLOG_DBG("done gracefulShutdown()" );
    }
    return obj ;
}

NodeCntlPtr
SharedServiceMain::makeNodeCntl()
{
    const char* serviceRoot= AppConfig::instance()->getString(JQS_ROOT,".");
    const char* subDir="node_cntl";
    char dirPath[strlen(serviceRoot)+strlen(subDir)+strlen("/")+1];
    sprintf(dirPath,"%s/%s", serviceRoot,subDir);
    NodeCntlPtr gb = new NodeCntl( 
        AppConfig::instance()->getString( JQS_NODE_CNTL_DIR, dirPath ),
        AppConfig::instance()->getString( JQS_OOS_SUFFIX, ".oos" ));
    return gb;
}



