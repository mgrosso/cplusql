//
// server.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

#include "GenericServiceImpl.h"
using namespace std;
#include <iostream>

/*
class exmain {
	public:
		Mutex m;
		Condition c;
};

int pingCount=0;

frame_thread_return_t
func1( void *x ){
    exmain *ex = (exmain *)x;
    while(1) {
        fprintf(stderr,"func1\n");
        {
            MutexGuard mg( ex->m );
            pingCount++;
        }
        sleep( 5 );
    }
    ex->c.signal();
    fprintf(stderr,"func1 finished signaling, sleep for 5 more\n");
    sleep( 5 );
    fprintf(stderr,"func1 finished sleeping, \n");
    return FRAME_THREAD_DEFAULT_RETURN;
};
*/

int main (int argc, char* argv[])
{
  try {
    // First initialize the ORB, that will remove some arguments...
    CORBA::ORB_var orb =
      CORBA::ORB_init (argc, argv,
                       "" /* the ORB name, it can be anything! */);
    CORBA::Object_var poa_object =
      orb->resolve_initial_references ("RootPOA");
    PortableServer::POA_var poa =
      PortableServer::POA::_narrow (poa_object.in ());
    PortableServer::POAManager_var poa_manager =
      poa->the_POAManager ();
    poa_manager->activate ();

    // Create the servant
    Service_GenericServiceImpl genericServiceImpl;

    // Activate it to obtain the object reference
    Service::GenericService_var genericService =
      genericServiceImpl._this ();

    // Put the object reference as an IOR string
    CORBA::String_var ior = orb->object_to_string (genericService.in ());
    // Print it out!
    cout << ior.in () << endl;

/*
    cerr << "creating new exmain"<<endl;
	exmain *ex = new exmain();
    cerr << "creating new Thread"<<endl;
	Thread t( func1, ex );
    //cerr << "creating new MutexGuard"<<endl;
	//MutexGuard mg( ex->m );
    //cerr << "calling wait on the exmain's condition"<<endl;
	//ex->c.wait( mg );
*/
    cerr << "orb->run"<<endl;
    orb->run ();

    // Destroy the POA, waiting until the destruction terminates
    poa->destroy (1, 1);
    orb->destroy ();
  }
  catch (CORBA::Exception &) {
    cerr << "CORBA exception raised!" << endl;
  }
  return 0;
}
