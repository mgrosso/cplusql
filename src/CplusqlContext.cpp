#ident "file_id $Id: CplusqlContext.cpp,v 1.30 2005/10/17 10:38:00 mgrosso Exp $"
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CplusqlContext.cpp
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 17 02:48:15 EST 2003 on circe.looksmart.net
// Project:
// Purpose:
//
// Copyright (c) 2003 LookSmart.
//
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 2 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 59 Temple
// Place, Suite 330, Boston, MA 02111-1307 USA
//
// $Id: CplusqlContext.cpp,v 1.30 2005/10/17 10:38:00 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------


#include "CplusqlContext.h"
#include "ptr_strdup.h"
#include "ptr.h"
#include "hashFuncs.h"
#include "AbstractRelation.h"
#include "BaseException.h"
#include "AsyncRunnable.h"
#include "AsyncRunnableCollection.h"
#include "SyncRunnableCollection.h"

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "SharedServiceMain.h"
#include "JobQueueServiceC.h"
#endif
#endif

#include "StringArrayRelation.h"

#include <map>
#include <time.h>

#include "cplusql_api.h"
#include "keys.h"
#include "frame_keys.h"
#include "AppLog.h"
#include "AppConfig.h"



typedef map< const char *, RelationPtr, cc_less_than > relmap_t;
typedef map< const char *, AbstractDelimitedSourceMapPtr, cc_less_than > ads_map_t;
typedef map< const char *, DBConnectionPtr, cc_less_than > connectionmap_t;
typedef map< const char *, ConfigPtr, cc_less_than > config_map_t;
typedef map< const char *, AbstractKeyValMapPtr, cc_less_than > kv_map_t;
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
typedef map< const char *, JobQRunnablePtr, cc_less_than > job_map_t;
typedef map< const char *, clusterio::NameSpaceService_ptr, cc_less_than > nss_map_t;
#endif
#endif
typedef map< const char *, ptr<DestNotifier>, cc_less_than > cc_destn_t;
typedef map< const char *, pair_array_meta_t, cc_less_than > array_map_t ;
typedef map< const char *, IndexPtr, cc_less_than > index_map_t ;

static const char *const globalstr="global" ;
static const char *const envstr="env" ;

CplusqlContext * CplusqlContext::instance_ = NULL ;

class CplusqlContextPriv 
{
    private:
    friend class CplusqlContext ;
    relmap_t                relations_;
    array_map_t             arrays_ ;
    index_map_t             indices_ ;
    ads_map_t               mir_;
    char_ptr_list           names_ ;
    list< ListenerPtr >     shims_ ;
    AsyncRunnableCollectionPtr   runlist_ ;
    ExpressionList          user_columns_ ;
    ptr< char_ptr_list >    user_colnames_ ;
    MetricList              metrics_ ;
    ptr< char_ptr_list >    metric_colnames_ ;
    config_map_t            configs_ ;
    kv_map_t				maps_ ;
    connectionmap_t	        connections_map_ ;
    char                    current_rel_[VARBUFLEN];
    char                    current_ads_map_[VARBUFLEN];
    RelationPtr             current_rel_t_;
    cc_destn_t              destn_ ;

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
	job_map_t               jobQ_;
	nss_map_t               nameSpace_;
#endif
#endif

    inline const char *     addName( const char *name )
    {
        char_ptr n=ptr_strdup( name );
        names_.push_front( n );
        return n.get();
    };
};

CplusqlContext::CplusqlContext()
{
    d_=new CplusqlContextPriv();
    if(!d_)
    {
        PANIC("cant alloc priv data");
    }
    d_->user_columns_ = new expression_list_t ;
    d_->user_colnames_= new char_ptr_list ;
    d_->metrics_        = new metric_list_t ;
    d_->metric_colnames_= new char_ptr_list ;
    if( ! d_->user_columns_.get() || ! d_->user_colnames_.get() )
    {
        PANIC("cant alloc priv data members");
    }
    ConfigPtr global( new Config());
    ConfigPtr env( new Config());
    env->loadEnv(true);
    d_->configs_[globalstr]=global;
    d_->configs_[envstr]=env;
    d_->runlist_ = new AsyncRunnableCollection();
    memset(d_->current_rel_, '\0', VARBUFLEN );
}

void
CplusqlContext::exit()
{
	if(d_ != NULL)
	{
		delete d_;
	}
}

CplusqlContext::~CplusqlContext()
{
	exit();
}

void 
CplusqlContext::addRelation( const char *name, RelationPtr &rp )
{
    d_->relations_[ d_->addName(name) ] = rp;
}

void 
CplusqlContext::addADSMap( const char *name, AbstractDelimitedSourceMapPtr &rp )
{
    d_->mir_[ d_->addName(name) ] = rp;
}

void 
CplusqlContext::addDBConnection( const char *name, DBConnectionPtr &conn )
{
    d_->connections_map_[ d_->addName(name) ] = conn;
}

DBConnectionPtr &
CplusqlContext::getDBConnection( const char *name )
{
    connectionmap_t::iterator i = d_->connections_map_.find(name);
    if( i== d_->connections_map_.end())
    {
        PANICV( "map not found: %s", name );
    }
    return (*i).second ;
}

void 
CplusqlContext::addShim( ListenerPtr &rp )
{
    d_->shims_.push_front( rp );
}

void 
CplusqlContext::addMap( const char *name, AbstractKeyValMapPtr &esm )
{
    APPLOG_DBG("adding %s\n", name );
    const char *n = d_->addName(name);
    d_->maps_[ n ] = esm ;
}

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
void
CplusqlContext::addJobQ( const char *name, JobQRunnablePtr & q)
{
	d_->jobQ_[d_->addName(name)] = q;

    RunnablePtr r (q);
    addRunnable(r);
	APPLOG_DBG("Adding jobQ %s qsize=%d", name, d_->jobQ_.size());
}


JobQRunnablePtr &
CplusqlContext::getJobQ( const char *name)
{
	job_map_t::iterator i = d_->jobQ_.find(name);
	if (i != d_->jobQ_.end()) {
		return i->second ;
	}else{
        PANICV("no jobq named %s", name );
    }
    //not reached.
    return i->second ;
}

void
CplusqlContext::addNameSpace( const char *name, clusterio::NameSpaceService_ptr nss)
{
	d_->nameSpace_[d_->addName(name)] = nss;

	APPLOG_DBG("Adding nameSpace %s nss_size=%d", name, d_->nameSpace_.size());
}

clusterio::NameSpaceService_ptr
CplusqlContext::getNameSpace( const char *name)
{
	nss_map_t::iterator i = d_->nameSpace_.find(name);
	if (i == d_->nameSpace_.end()) 
    {
        return NULL;
	}
    return i->second;
}
#endif
#endif

AbstractKeyValMapPtr &
CplusqlContext::getMap( const char *name )
{
    kv_map_t::iterator i = d_->maps_.find(name);
    if( i== d_->maps_.end())
    {
        PANICV( "map not found: %s", name );
    }
    return (*i).second ;
}

void
CplusqlContext::addConfig( const char *name, ConfigPtr &rp )
{
    //we will happily trash any existing config with that name.
    d_->configs_[d_->addName(name) ] = rp;
}

ConfigPtr &
CplusqlContext::getConfig( const char *name )
{
    config_map_t::iterator i=d_->configs_.find( name );
    config_map_t::iterator e=d_->configs_.end();
    if( i == e )
    {
        PANICV( "no such config: %s", name );
    }
    return (*i).second;
}

ConfigPtr &
CplusqlContext::getDefaultConfig( )
{
    return d_->configs_[globalstr];
}


const char *
CplusqlContext::getUsageString()
{
    static const char *s=
"cplusql [options] [file]\n\n"
"    -f  <file>          process cplusql file <file> (- is stdin)\n"
"    --file=<file>       process cplusql file <file>\n"
"    -l or --lexdbg      turns on lex debugging (lexdbg)\n"
"    -y  or --yacdbg     turns on grammar debugging (yacdbg)\n"
"    -v                  turns on debug logging. (APPLOG_MINLOG=0)\n"
"    -t                  turns on timing information (SYNC_VERBOSE)\n"
"    -T                  prevents use of threads (CPLUSQL_NOTHREADS)\n"
"    -B                  prevents buffering of output (DEST_BUFFERED)\n"
"    -D                  debug. equivalent to -BTtlyv.\n"
"    -h or -? or anything else generates this message\n"
"\n    commands are read from stdin if not specified with -f or --file option\n" ;
    return s;
}
#define CPLUSQL_OPTARG "BDTtvlyh?f:d:"

bool
CplusqlContext::initConfig( int *argc, char **argv )
{
    ConfigPtr &global=d_->configs_[globalstr];
    global->loadEnv(true);
    global->consumeArgs( argc, argv );
    global->addString( "lexdbg", "0",true);
    global->addString( "yacdbg", "0",true);
    char arg;
    while(-1!=(arg=getopt( *argc,argv,CPLUSQL_OPTARG )))
    {
        switch (arg)
        {
            case 'D':
                global->addString( "SYNC_VERBOSE", "1", true );
                global->addString( "APPLOG_MINLOG", "0", true );
                global->addString( CPLUSQL_NOTHREADS, "1", true );
                global->addString( DEST_BUFFERED, "0",true );
                global->addString( "lexdbg", "1",true);
                global->addString( "yacdbg", "1",true);
                break;
            case 'B':
                global->addString( DEST_BUFFERED, "0",true );
                break;
             case 'T':
                global->addString( CPLUSQL_NOTHREADS, "1",true );
                break;
            case 't':
                global->addString( "SYNC_VERBOSE", "1", true );
                break;
            case 'v':
                global->addString( "SYNC_VERBOSE", "1", true );
                global->addString( APPLOG_MINLOG_CFKEY, "0", true );
                AppLog::instance()->reconfigure( *global );
                break;
            case 'l':
                global->addString( "lexdbg", "1",true);
                break;
            case 'y':
                global->addString( "yacdbg", "1",true);
                break;
            case 'f':
                if( optarg )
                {
                    global->addString("file",optarg,true);
                }
                else return false;
                break;
            case 'd':
                if( optarg )
                {
                    global->addString(CPLUSQL_DELIMITER,optarg,true);
                }
                else return false;
            case 'h':
            case '?':
            default:
                return false;
        }
    }
#define DO_APP_CONFIG 1
#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
	SharedServiceMain::init( *argc, argv, __FILE__, false );
#undef DO_APP_CONFIG
#endif
#endif
#ifdef DO_APP_CONFIG
    AppConfig::instance()->init( *argc, argv );
    AppConfig::instance()->addString(   "APPLOG_MINLOG", "1", 0 );
#endif

    if( !global->findString(CPLUSQL_DELIMITER))
    {
        global->addString(CPLUSQL_DELIMITER,CPLUSQL_DELIMITER_DFLT,false);
    }
    if( global->getBool( CPLUSQL_NOTHREADS, CPLUSQL_NOTHREADS_DFLT ))
    {
        global->addString( SRC_THREADED, "0", true );
        global->addString( DEST_THREADED, "0", true );
        global->addString( APPLOG_ASYNC_CFKEY, "0", true );
    }
    set_lexdbg( static_cast<int>(global->getBool( "lexdbg", "false")));
    set_yacdbg( static_cast<int>(global->getBool( "yacdbg", "false")));
    return true;
}

void
CplusqlContext::addRunnable( AsyncRunnablePtr &r )
{
    d_->runlist_->add( r );
}

void
CplusqlContext::addRunnable( RunnablePtr &r )
{
    d_->runlist_->addThread( r );
}

void
CplusqlContext::addArray( const char *name, StringArrayPtr &array, RelationMetaPtr &meta )
{
    pair_array_meta_t p(array, meta );
    d_->arrays_[d_->addName(name) ] = p;
}

void
CplusqlContext::addIndex( const char *name, IndexPtr &index )
{
    d_->indices_[d_->addName(name) ] = index ;
}

pair_array_meta_t &
CplusqlContext::getArray( const char *name )
{
    array_map_t::iterator i = d_->arrays_.find( name );
    if( i == d_->arrays_.end())
    {
        PANICV("array %s not found", name );
    }
    return (*i).second ;
}

IndexPtr &
CplusqlContext::getIndex( const char *name )
{
    index_map_t::iterator i = d_->indices_.find( name );
    if( i == d_->indices_.end())
    {
        PANICV("index %s not found", name );
    }
    return (*i).second ;
}


RelationPtr &
CplusqlContext::getRelation( const char *name )
{
    if(!name || !*name )
    {
        PANICV( "no such relation[%s]", name );
    }
    relmap_t::iterator i = d_->relations_.find(name);
    if( i== d_->relations_.end())
    {
        array_map_t::iterator j= d_->arrays_.find(name);
        if( j== d_->arrays_.end())
        {
            PANICV( "relation not found: %s", name );
        }
        ptr< StringArrayRelation > sarp = 
            new StringArrayRelation((*j).second.second,(*j).second.first);
        RelationPtr rlp( sarp );
        RunnablePtr rnp( sarp );
        addRelation( name, rlp );
        addRunnable( rnp );
        return getRelation( name );
	}
    return ((*i).second);
}

bool
CplusqlContext::haveRelation( const char *name )
{
    relmap_t::iterator i = d_->relations_.find(name);
    if( i== d_->relations_.end())
    {
        array_map_t::iterator j= d_->arrays_.find(name);
        if( j== d_->arrays_.end())
        {
            return false ;
        }
	}
    return true ;
}


void            
CplusqlContext::addDestNotifier( 
    const char *name, ptr< DestNotifier > &destn )
{
    d_->destn_[d_->addName(name) ] = destn ;
}

ptr< DestNotifier > & 
CplusqlContext::getDestNotifier( const char *name)
{
    cc_destn_t::iterator i = d_->destn_.find(name);
    if( i== d_->destn_.end())
    {
        PANICV( "%s not found", name );
	}
    return ((*i).second);
}

AbstractDelimitedSourceMapPtr &
CplusqlContext::getADSMap( const char *name )
{
    ads_map_t::iterator i = d_->mir_.find(name);
    if( i== d_->mir_.end())
    {
        PANICV( "%s not found", name );
    }
    return ((*i).second);
}

void
CplusqlContext::setCurrentSrcADSMap( const char * name )
{
    if (strlen(name) >= VARBUFLEN -1 )
    {
        PANICV("ADSMap name exceeds max allowable length for an ads map name: %s", name );
    }
    memset(d_->current_ads_map_, '\0', VARBUFLEN);
    strcpy(d_->current_ads_map_, name);
}

AbstractDelimitedSourceMapPtr &
CplusqlContext::getCurrentSrcADSMap()
{
    return getADSMap( d_->current_ads_map_ );
}

void
CplusqlContext::setCurrentSrcRelation( const char * name )
{
    if (strlen(name) >= VARBUFLEN -1 )
    {
        PANICV("Relation name exceeds max allowable length of %u: %s ", name );
    }
    memset(d_->current_rel_, '\0', VARBUFLEN);
    strcpy(d_->current_rel_, name);
}

RelationPtr &
CplusqlContext::getCurrentSrcRelation()
{
    if(!*d_->current_rel_)
    {
        PANICV("you requested a column but no source relation is defined yet. its could also be that you used a mistyped a function name.");
    }
    return getRelation( d_->current_rel_ );
}

void 
CplusqlContext::pushUserColumn( char_ptr &name, ExpressionPtr &ep )
{
    d_->user_columns_->push_back( ep );
    d_->user_colnames_->push_back( name );
}

ExpressionList 
CplusqlContext::getUserColumns()
{
    ExpressionList gb=d_->user_columns_;
    d_->user_columns_= new expression_list_t ;
    return gb ;
}

ptr< char_ptr_list >
CplusqlContext::getUserColumnNames()
{
    ptr< char_ptr_list> gb = d_->user_colnames_ ;
    d_->user_colnames_= new char_ptr_list ;
    return gb;
}

void 
CplusqlContext::pushMetric( char_ptr &name, AbstractProjectionPtr &ep )
{
    d_->metrics_->push_back( ep );
    d_->metric_colnames_->push_back( name );
}

MetricList 
CplusqlContext::getMetrics()
{
    MetricList gb=d_->metrics_;
    d_->metrics_= new metric_list_t ;
    return gb ;
}

ptr< char_ptr_list >
CplusqlContext::getMetricNames()
{
    ptr< char_ptr_list> gb = d_->metric_colnames_ ;
    d_->metric_colnames_= new char_ptr_list ;
    return gb;
}

CplusqlContext & 
CplusqlContext::instance()
{
    if( NULL == instance_ )
    {
        instance_ = new CplusqlContext();
    }
    return *instance_ ;
}

bool
CplusqlContext::run()
{
    AppLog::instance()->reconfigure( * getDefaultConfig());
    APPLOG_INFO("sync start...");

    bool success = true;
    if( getDefaultConfig()->getBool( "CPLUSQL_NOTHREADS", "0" ))
    {
        SyncRunnableCollection sync( d_->runlist_->getRunnables());
        sync.run(); 
        //exceptions propogate and handled elsewhere, so no need to
        //change success bool.
    }
    else
    {
        d_->runlist_->start();
        if( AsyncRunnable::failed == d_->runlist_->wait())
        {
            success = false;
            APPLOG_ABORT("error: %s\n", d_->runlist_->getErrorString());
        }
    }
    //make sure all ptr have opportunity to delete all non-string objects.
    //configs can be left behind.
    //fprintf( stderr, "%s\n", ctime(&t));
    APPLOG_INFO("sync work done, cleaning up objects");
    d_->runlist_ = new AsyncRunnableCollection(); 
    d_->user_columns_->clear();
    d_->relations_.clear();
    d_->shims_.clear();
    d_->relations_.clear();
    memset(d_->current_rel_, '\0', VARBUFLEN );
    APPLOG_INFO("sync complete.");
    return success; 
}

