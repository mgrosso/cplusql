// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      CplusqlContext.h
// Author:    mgrosso Matthew Grosso
// Created:   Mon Feb 17 02:10:40 EST 2003 on circe.looksmart.net
// Project:
// Purpose:
//
// Copyright (c) 2003 LookSmart.  contact mgrosso@acm.org
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
// $Id: CplusqlContext.h,v 1.20 2005/10/17 11:18:58 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef CPLUSQLCONTEXT_H
#define CPLUSQLCONTEXT_H 1

#include "AbstractRelation.h"
#include "AbstractJoint.h"
#include "ptr_strdup.h"
#include "Expression.h"
#include "AbstractProjection.h"
#include "Config.h"
#include "AbstractDelimitedSource.h"
#include "AbstractDelimitedSourceMap.h"
#include "AbstractKeyValMap.h"
#include "AsyncRunnable.h"
#include "AsyncRunnableCollection.h"
#include "db/DBConnection.h"
#include "DestNotifier.h"
#include "StringArray.h"
#include "Index.h"
#include "RelationMeta.h"


#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
#include "JobQRunnable.h"
#include "JobQueueServiceC.h"
#include "clusterioC.h"
#endif
#endif

typedef pair< StringArrayPtr, RelationMetaPtr > pair_array_meta_t ;

class CplusqlContextPriv ;
class CplusqlContext
{
    public:
        static CplusqlContext & instance();
        ~CplusqlContext();

        void            exit();

        void            addMap( const char *name, AbstractKeyValMapPtr &esmp );
        AbstractKeyValMapPtr &
                        getMap( const char *name );

        void            addShim( ListenerPtr &rp );
        void            addRelation( const char *name, RelationPtr &rp );
        void            addArray( const char *name,
                            StringArrayPtr &array, RelationMetaPtr &meta );
        pair_array_meta_t & getArray( const char *name );
        IndexPtr &      getIndex( const char *name );

        void            addIndex( const char *name, IndexPtr &index );
        void            addADSMap( const char *name, AbstractDelimitedSourceMapPtr &rp );
        void            addDBConnection( const char *name, DBConnectionPtr &conn );
		DBConnectionPtr & getDBConnection( const char *name );

        RelationPtr &   getRelation( const char *name );
        bool            haveRelation( const char *name );
        AbstractDelimitedSourceMapPtr &   getADSMap( const char *name );
        AbstractDelimitedSourceMapPtr &   getCurrentSrcADSMap();
        RelationPtr &   getCurrentSrcRelation();
        void            setCurrentSrcRelation( const char * name );
        void            setCurrentSrcADSMap( const char * name );

        void            addDestNotifier( const char *name,
                            ptr< DestNotifier > &destn );
        ptr< DestNotifier > & getDestNotifier( const char *name);

        bool            run();
        void            addRunnable( AsyncRunnablePtr &work );
        void            addRunnable( RunnablePtr &work );

#ifdef BUILD_SERVICES
#if BUILD_SERVICES = 1
		void            addJobQ( const char *name, JobQRunnablePtr &jq);
		JobQRunnablePtr&getJobQ( const char *name);
		void            addNameSpace( const char *name, clusterio::NameSpaceService_ptr nss);
		clusterio::NameSpaceService_ptr getNameSpace( const char *name);
#endif
#endif


        void            pushUserColumn( char_ptr &name, ExpressionPtr &ep );
        ExpressionList  getUserColumns();
        ptr< char_ptr_list > getUserColumnNames();
        void            pushMetric( char_ptr &name, AbstractProjectionPtr &ep );
        MetricList  getMetrics();
        ptr< char_ptr_list > getMetricNames();

        void            addConfig( const char *name, ConfigPtr &pc );
        ConfigPtr      &getConfig( const char *name );
		ConfigPtr      &getDefaultConfig();
        bool            initConfig( int *argc, char **argv );

        static const char * getUsageString();

    private:
        void init_usercol();

        CplusqlContext();
        CplusqlContextPriv *d_;
        static CplusqlContext *instance_ ;

        //not implemented
        CplusqlContext( const CplusqlContext &rhs);
        CplusqlContext &operator=( const CplusqlContext &rhs);
};

#endif /* CPLUSQLCONTEXT_H */
