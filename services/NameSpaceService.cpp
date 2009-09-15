//
// server.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

#include "NameSpaceServiceImpl.h"
#include "SharedServiceMain.h"
#include "keys.h"

static PortableServer::Servant 
nssFactory()
{
    static NameSpaceServiceImpl nss;
    return &nss;
};

int main (int argc, char* argv[])
{
    return SharedServiceMain::serviceMain(
        argc, argv, NSS_NAME, nssFactory, 
        true, true, NSS_IORFILE, RWS_NAME );
};

