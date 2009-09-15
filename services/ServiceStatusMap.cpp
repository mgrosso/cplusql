// -*- C++ -*-
//
// $Id: ServiceStatusMap.cpp,v 1.38 2004/08/05 03:52:45 mgrosso Exp $

#include <stdio.h>
#include "ServiceStatusMap.h"
#include "BaseException.h"
#include "Exclusion.h"
#include "AppLog.h"
#include "NVPairUtil.h"
#include "ptr_strdup.h"
#include "hashFuncs.h"
#include "keys.h"

typedef map<const char_ptr, ServiceStatusPtr, char_ptr_less_than > ServiceStatusMapType;

class ServiceStatusMapPriv
{
private:
    friend class ServiceStatusMap;

    ServiceStatusMapPriv()
    {
        ;//noop
    };

    ServiceStatusMapType serviceStatusMap;
    Mutex lock;
};

ServiceStatusMap::ServiceStatusMap (void) 
{
    d_=new ServiceStatusMapPriv();
    if(!d_)
    {
        //this may not be necssary as it should throw an std::Exception
        THROW_NOMEM;
    }
  }
  
// Implementation skeleton destructor
ServiceStatusMap::~ServiceStatusMap(void)
{
    delete d_;
}

long ServiceStatusMap::size(const MutexGuard* existingMutexGuard)
{
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    return d_->serviceStatusMap.size();
}

unsigned int
ServiceStatusMap::countLive( const char *service, const NodeCntl *ncntl,
    const MutexGuard* existingMutexGuard )
{
    Service::ServiceStatusList *sl ;
    {
        ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
        sl = elements( service, "*", pMutexGuard.get() );
    }
    ptr< Service::ServiceStatusList > psl(sl);
    unsigned int length = sl->length();
    unsigned int gb = 0;
    for (unsigned int i=0; i<length; i++) {
        if( (*sl)[i].isPendingShutdown == false &&
            (*sl)[i].live == true && 
            ncntl->isOK( (*sl)[i].ref.hostname.in()))
        {
            ++gb;
        }
    }
    return gb;
}

void ServiceStatusMap::pushStatusList (
    const Service::ServiceStatusList & serviceStatusList,
    const MutexGuard* existingMutexGuard
  )
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    int length = serviceStatusList.length();
    for (int i=0; i<length; i++) {
        
        Service::ServiceStatus* newServiceStatus =
            new Service::ServiceStatus( serviceStatusList[i] );

        newServiceStatus->statusReceivedTime = tv.tv_sec;
        ServiceStatusPtr pServiceStatus(newServiceStatus);

        char_ptr pURL = ptr_strdup(newServiceStatus->ref.url);
        d_->serviceStatusMap[pURL] = pServiceStatus;
    }
}

ServiceStatusPtr ServiceStatusMap::get(const char_ptr& pURL, const MutexGuard* existingMutexGuard)
{
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    ServiceStatusPtr pServiceStatus;
    if(d_->serviceStatusMap.find(pURL) != d_->serviceStatusMap.end())
    {
        pServiceStatus = d_->serviceStatusMap[pURL];
    }
    return pServiceStatus;
}

void
ServiceStatusMap::modifyNode(
	Service::ServiceStatus *jobexec, int used, int available )
{
	ConfigPtr pDetailsConfig =
			NVPairUtil::convert(jobexec->details);
	char_ptr pUsedSlots = ptr_strdup_long( used );
	char_ptr pAvailableSlots = ptr_strdup_long( available );

	pDetailsConfig->addString(
		JE_USED_SLOTS, pUsedSlots.get(), true );
	pDetailsConfig->addString(
		JE_AVAILABLE_SLOTS,
		pAvailableSlots.get(), true );

	// convert the details back
	Service::NVPairList* details =
		NVPairUtil::convert(*pDetailsConfig);

	jobexec->details = *details;
	delete details ;
}

void
ServiceStatusMap::readNode(
	Service::ServiceStatus *jobexec, int &used, int &available )
{
	ConfigPtr pDetailsConfig = NVPairUtil::convert(jobexec->details);

	used = pDetailsConfig->getInt( JE_USED_SLOTS );
	available = pDetailsConfig->getInt( JE_AVAILABLE_SLOTS );
}


void ServiceStatusMap::put(const char_ptr& pURL, ServiceStatusPtr& serviceStatusPtr, const MutexGuard* existingMutexGuard)
{
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    d_->serviceStatusMap[pURL]=serviceStatusPtr;
}

void ServiceStatusMap::evaluateLiveness(int tolerance, const MutexGuard* existingMutexGuard)
{
    APPLOG_DBG("in evaluateLiveness()");
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);

    struct timeval tv;
    struct timezone tz;

    gettimeofday(&tv, &tz);

    ServiceStatusMapType::iterator current, e ;
    for (current = d_->serviceStatusMap.begin(), e = d_->serviceStatusMap.end();
         current !=e ;
         ++ current) {

        ServiceStatusPtr& pServiceStatus = (*current).second;
    
        if (tv.tv_sec - pServiceStatus->statusReceivedTime > tolerance) {
            pServiceStatus->live = false;
            APPLOG_DBG("live=false (tv.tv_sec=%d, pServiceStatus.get()->statusReceivedTime=%d:",tv.tv_sec, pServiceStatus.get()->statusReceivedTime);
            APPLOG_WARN("%s %s is no longer live (last received msg was %ld seconds ago)",
            
            pServiceStatus->ref.serviceName.in(),
            pServiceStatus->ref.url.in(),
            (tv.tv_sec - pServiceStatus->statusReceivedTime));
            printServiceStatus(pServiceStatus.get());
        }
    }
}

MutexGuard* ServiceStatusMap::checkMutexGuard(const MutexGuard* existingMutexGuard)
{
    MutexGuard* mg=NULL;
    
    if (existingMutexGuard != NULL)
    {
        frame_mutex_t* existingMutex = existingMutexGuard->getMutex();
        //APPLOG_DBG("existingMutexGuard->getMutex()=%d", existingMutex);
        //APPLOG_DBG("d_->lock.getMutex()=%d", d_->lock.getMutex());
        if (existingMutex != d_->lock.getMutex())
        {
            APPLOG_DBG("invalid passed lock");
            PANICV("mutex != d_->lock.getMutex()");
        }
        //APPLOG_DBG("using passed lock");
    }
    else
    {
        //APPLOG_DBG("attempting to acquire new mutex");
        mg = new MutexGuard(d_->lock);
        //APPLOG_DBG("got mutex");
    }
    return mg;
}

Service::ServiceStatusList* ServiceStatusMap::elements(const char* serviceName,
                                                       const char* host,
                                                       const MutexGuard* existingMutexGuard)
{
    APPLOG_DBG("in ServiceStatusMap::elements(%s, %s)", serviceName, host);
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);

    int mapsize = d_->serviceStatusMap.size();
    APPLOG_DBG("mapsize=%d",mapsize);
    
    Service::ServiceStatus serviceStatus[mapsize];

    int matchingCount=0;
    ServiceStatusMapType::iterator current, e ;
    for (current = d_->serviceStatusMap.begin(), e = d_->serviceStatusMap.end();
         current !=e ;
         ++ current) {

        ServiceStatusPtr& pServiceStatus = (*current).second;
        if ((!strcmp(host, "*") || !strcmp(pServiceStatus->ref.hostname.in(), host))
            &&
            (!strcmp(serviceName, "*") || !strcmp(pServiceStatus->ref.serviceName, serviceName)))
        {
            serviceStatus[matchingCount++]=*(pServiceStatus.get());
        }
        printServiceStatus(pServiceStatus.get());
    }

    Service::ServiceStatus* matchingServiceStatus = new Service::ServiceStatus[matchingCount];
    for (int i=0; i<matchingCount; i++)
    {
        matchingServiceStatus[i] = serviceStatus[i];
    }

    Service::ServiceStatusList *serviceStatusList =
        new Service::ServiceStatusList(matchingCount,matchingCount,matchingServiceStatus, 1);

    APPLOG_DBG("returning from serviceStatusList with %d matches", matchingCount);
    return serviceStatusList;
}

void ServiceStatusMap::printServiceStatusMap (const MutexGuard* existingMutexGuard) 
{
    ptr<MutexGuard> pMutexGuard = checkMutexGuard(existingMutexGuard);
    
    ServiceStatusMapType::iterator current, e ;
    for (current = d_->serviceStatusMap.begin(), e = d_->serviceStatusMap.end();
         current !=e ;
         ++ current) {

        ServiceStatusPtr& pServiceStatus = (*current).second;
        printServiceStatus(pServiceStatus.get());
    }
}
  
void ServiceStatusMap::printServiceStatus(const Service::ServiceStatus* serviceStatus) 
{
    
    APPLOG_DBG("url=%s|serviceName=%s|host=%s|pid=%d|statusReceivedTime=%d|isPendingShutdown=%d|live=%d", serviceStatus->ref.url.in(), serviceStatus->ref.serviceName.in(), serviceStatus->ref.hostname.in(), serviceStatus->pid, serviceStatus->statusReceivedTime, serviceStatus->isPendingShutdown, serviceStatus->live);

    ConfigPtr pDetailsConfig = 
        NVPairUtil::convert(serviceStatus->details);
    ptr< char_ptr_vec > keys=pDetailsConfig->getKeys();

    char_ptr_vec::iterator end=keys->end();
    for (char_ptr_vec::iterator current = keys->begin();
         current != end;
         current++)
    {
        char_ptr pKey = *current;
        char* key = pKey.get();
        APPLOG_DBG("url=%s|%s=%s", serviceStatus->ref.url.in(), key, pDetailsConfig->getString(key));
    }
}

Mutex* ServiceStatusMap::getMutex() const
{
    return &(d_->lock);
}
