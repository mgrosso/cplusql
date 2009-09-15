//
// server.cpp,v 1.4 2001/03/29 19:42:06 coryan Exp
//

#include "JobExecServiceImpl.h"
#include "SharedServiceMain.h"
#include "keys.h"

static PortableServer::Servant 
jeFactory()
{
    static JobExec_JobExecService_i je(*AppConfig::instance());
    return &je;
};

int main (int argc, char* argv[])
{
    return SharedServiceMain::serviceMain(
        argc, argv, JE_NAME, jeFactory, false, true );
};

