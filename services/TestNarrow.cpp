#ident "TestNarrow.cpp $Id: TestNarrow.cpp,v 1.1 2004/08/30 19:23:32 holahrei Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      TestNarrow.cpp
// Author:    mgrosso Matt Grosso
// Created:   Sat Feb 28 11:45:19 PST 2004 on sf-devdw012.looksmart.com
// Project:   
// Purpose:   
//
// Copyright (c) 2004 LookSmart. All Rights Reserved.
//
// $Id: TestNarrow.cpp,v 1.1 2004/08/30 19:23:32 holahrei Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

#include "TestNarrow.h"

#include "tao/corba.h"
#include "tao/ORB.h"
#include "tao/PortableServer/POA.h"
#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/orbsvcs/CosNamingC.h"

TestNarrow::TestNarrow( 
    int argc, char **argv)
{
    try{

        if (argc < 2)
        {
            printf("usage: TestNarrow NAMESERVICE_HOST NAMESERVICE_PORT [LOOPCOUNT]");
            exit(1);
        }

        CORBA::ORB_var
            orb_ = CORBA::ORB_init( argc, argv, "TestNarrow_orb_" );

        CORBA::Object_var poa_object = 
            orb_->resolve_initial_references( "RootPOA" );

        PortableServer::POA_var
            poa_ = PortableServer::POA::_narrow (poa_object.in ());

        PortableServer::POAManager_var
            poa_manager_ =  poa_->the_POAManager (); 

        poa_manager_->activate();

        static int namebuflen = 1024 ;
        char namebuf [ namebuflen + 1] ;
        namebuf[ namebuflen ] = '\0' ;
        char* nshost=argv[1];
        char* nsport=argv[2];
        snprintf(namebuf, namebuflen, "iiop://%s:%s/NameService",
                    nshost, nsport);
        printf("%d %s %s %s %s\n", argc, argv[0],argv[1],argv[2],argv[3]);
        char* loopCountEnv = 0;
        if(argc > 2)
        {
            loopCountEnv = argv[3];
        }

        int loopCount = (loopCountEnv == 0 ? 1 : atoi(loopCountEnv));

        for (int i=0; i < loopCount; i++)
        {
            printf("Entering loop %d\n", i);
            object_var_narrow_var(orb_, namebuf);

            object_ptr_narrow_ptr(orb_, namebuf, 0, 0);
            object_ptr_narrow_ptr(orb_, namebuf, 0, 1);
            object_ptr_narrow_ptr(orb_, namebuf, 1, 0);
            object_ptr_narrow_ptr(orb_, namebuf, 1, 1);

            object_ptr_narrow_var(orb_, namebuf, 0);
            object_ptr_narrow_var(orb_, namebuf, 1);
            object_var_narrow_ptr(orb_, namebuf, 0);
            object_var_narrow_ptr(orb_, namebuf, 1);
            
        }
        poa_->destroy(1,1);
        orb_->destroy();
    }
    catch (CORBA::Exception &ce) 
    {
        printf("TestNarrow main() caught CORBA::Exception [%s][%s]\n", 
            ce._rep_id(), ce._name() );
        exit(1);
    }
    catch (std::exception &e) 
    {
        printf("TestNarrow main() caught std::exception\n");
        //printf("TestNarrow main() caught std::exception [%s][%s]\n", 
        //    e._rep_id(), e._name() );
        exit(1);
    }
    catch ( ... )
    {
        printf("TestNarrow main() caught exception\n");
        //printf("TestNarrow main() caught exception [%s][%s]\n", 
        //    ce._rep_id(), ce._name() );
        exit(1);
    }
}

TestNarrow::~TestNarrow()
{
}

void
TestNarrow::object_var_narrow_var(CORBA::ORB_var& orb_, char* url)
{
    printf("ctor() retrieving nameservice object %s ... ", url);
    // using var
    CORBA::Object_var naming_context_object_var =
        orb_->string_to_object (url );

    if( ! naming_context_object_var )
    {
        printf( "cant string_to_object nameservice ref: %s ", url );
        exit (1);
    }

    printf("ctor() narrowing nameservice object %s %x ... \n", url, naming_context_object_var.in());

    // using var:
    CosNaming::NamingContext_var naming_context_var =
        CosNaming::NamingContext::_narrow (naming_context_object_var.in ());

    printf("ctor() narrowed nameservice object %s to %x ... \n", url, naming_context_var.in());
    if( ! naming_context_var )
    {
        printf( "cant narrow nameservice obj: %s \n", url );
        exit (1);
    }

    printf("naming_context_object_var=%x, naming_context_var=%x\n",naming_context_object_var.in(),naming_context_var.in());
}

void
TestNarrow::object_ptr_narrow_ptr(CORBA::ORB_var& orb_, char* url, bool release_object, bool release_narrowed)
{
    printf("ctor() retrieving nameservice object %s ... ", url);
    CORBA::Object_ptr naming_context_object_ptr =
        orb_->string_to_object (url );

    if( ! naming_context_object_ptr )
    {
        printf( "cant string_to_object nameservice ref: %s\n", url );
        exit (1);
    }

    printf("ctor() narrowing nameservice object %s %x ... \n", url, naming_context_object_ptr);

    //using ptr:
    CosNaming::NamingContext_ptr naming_context_ptr =
        CosNaming::NamingContext::_narrow (naming_context_object_ptr);

    printf("ctor() narrowed nameservice object %s to %x ... \n", url, naming_context_ptr);

    if( ! naming_context_ptr )
    {
        printf( "cant narrow nameservice obj: %s \n", url );
        exit (1);
    }

    printf("naming_context_object_ptr=%x, naming_context_ptr=%x\n",naming_context_object_ptr,naming_context_ptr);
    
    if(release_object)
    {
        printf("calling CORBA::release(naming_context_ptr:%x)\n", naming_context_object_ptr);
        CORBA::release(naming_context_object_ptr);
    }
    if(release_narrowed)
    {
        printf("calling CORBA::release(naming_context_ptr:%x)\n", naming_context_ptr);
        CORBA::release(naming_context_ptr);
    }
}

void
TestNarrow::object_ptr_narrow_var(CORBA::ORB_var& orb_, char* url, bool release_object)
{
    printf("ctor() retrieving nameservice object %s ... ", url);
    CORBA::Object_ptr naming_context_object_ptr =
        orb_->string_to_object (url );

    if( ! naming_context_object_ptr )
    {
        printf( "cant string_to_object nameservice ref: %s\n", url );
        exit (1);
    }

    printf("ctor() narrowing nameservice object %s %x ... \n", url, naming_context_object_ptr);

    //using var:
    CosNaming::NamingContext_var naming_context_var =
        CosNaming::NamingContext::_narrow (naming_context_object_ptr);

    printf("ctor() narrowed nameservice object %s to %x ... \n", url, naming_context_var.in());

    if( ! naming_context_var )
    {
        printf( "cant narrow nameservice obj: %s \n", url );
        exit (1);
    }

    printf("naming_context_object=%x, naming_context=%x\n",naming_context_object_ptr,naming_context_var.in());
    
    if(release_object)
    {
        printf("calling CORBA::release(naming_context_ptr:%x)\n", naming_context_object_ptr);
        CORBA::release(naming_context_object_ptr);
    }
}

void
TestNarrow::object_var_narrow_ptr(CORBA::ORB_var& orb_, char* url, bool release_narrowed)
{
    printf("ctor() retrieving nameservice object %s ... ", url);
    // using var
    CORBA::Object_var naming_context_object_var =
        orb_->string_to_object (url );

    if( ! naming_context_object_var )
    {
        printf( "cant string_to_object nameservice ref: %s", url );
        exit (1);
    }

    printf("ctor() narrowing nameservice object %s %x ... \n", url, naming_context_object_var.in());

    //using ptr:
    CosNaming::NamingContext_ptr naming_context_ptr =
        CosNaming::NamingContext::_narrow (naming_context_object_var.in ());

    printf("ctor() narrowed nameservice object %s to %x ... \n", url, naming_context_ptr);

    if( ! naming_context_ptr )
    {
        printf( "cant narrow nameservice obj: %s \n", url );
        exit (1);
    }
    
    printf("naming_context_object=%x, naming_context=%x\n",naming_context_object_var.in(),naming_context_ptr);
    
    if(release_narrowed)
    {
        printf("calling CORBA::release(naming_context_ptr:%x)\n", naming_context_ptr);
        CORBA::release(naming_context_ptr);
    }
}

int main (int argc, char* argv[])
{
    TestNarrow testNarrow(argc, argv);
};
