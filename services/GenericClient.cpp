#ident "file_id $Id: GenericClient.cpp,v 1.4 2004/05/24 10:15:54 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      GenericClient.cpp
// Author:    mgrosso 
// Created:   Thu May 20 17:24:25 EDT 2004 on caliban
// Project:   
// Purpose:   
// 
// $Id: GenericClient.cpp,v 1.4 2004/05/24 10:15:54 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

//c++ includes
#include <exception>

//system includes

//frame includes
#include "AppLog.h"

//idl generated includes
#include "serviceC.h"

//cplusql includes
#include "SharedServiceMain.h"

int
main( int argc, char **argv )
{
    try{
        SharedServiceMain::init( argc, argv, __FILE__, false );
        (void)SharedServiceMain::genericClientMain();
        return 0;
    }
    catch (Service::ServiceException &se) 
    {
        APPLOG_ABORT( "main() caught System::SystemException [%s]", 
            se.what_string.in());
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

