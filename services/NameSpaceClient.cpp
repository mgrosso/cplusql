#ident "file_id $Id: NameSpaceClient.cpp,v 1.1 2004/05/24 10:17:04 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      NameSpaceClient.cpp
// Author:    mgrosso 
// Created:   Thu May 20 17:24:25 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: NameSpaceClient.cpp,v 1.1 2004/05/24 10:17:04 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

//frame includes
#include "AppConfig.h"
#include "ConfigDump.h"
#include "Slurp.h"

//idl generated includes
#include "clusterioC.h"

//cplusql includes
#include "SharedServiceMain.h"
#include "NVPairUtil.h"
#include "keys.h"

using namespace clusterio ;

int
main( int argc, char **argv )
{
    try{
        SharedServiceMain::init( argc, argv, __FILE__, false );
        CORBA::Object_ptr obj = SharedServiceMain::genericClientMain();
        APPLOG_DBG("converting obj to nss" );
        NameSpaceService_ptr nss = NameSpaceService::_narrow( obj );
        if( !nss )
        {
            PANICV( "could not narrow object to NameSpaceService: %s", 
            SharedServiceMain::instance()->getORB()->object_to_string(obj)
            );
        }
        return 0;
    }
    catch (Service::ServiceException &se) 
    {
        APPLOG_ABORT( "main() caught System::SystemException [%s]", 
            se.what_string.in());
        ;
    }
    catch (CORBA::Exception &ce) 
    {
        APPLOG_ABORT( "main() caught CORBA::Exception [%s][%s]", 
            ce._rep_id(), ce._name() );
    }
    catch (std::exception &e) 
    {
        APPLOG_ABORT("main() caught std::exception [%s]", e.what());
    }
    catch ( ... )
    {
        APPLOG_ABORT("main() caught unknown exception" );
    }
    return 1;
}

