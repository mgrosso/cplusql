// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      keys.h
// Author:    mgrosso Matt Grosso
// Created:   Wed Mar 17 16:47:22 PST 2004 on sf-devdw012.looksmart.com
// Project:   cplusql corba services
// Purpose:   one place to define what the configuration key strings are.
// 
// Copyright (c) 2004 LookSmart. All Rights Reserved.
// 
// $Id: keys.h,v 1.34 2005/03/11 19:57:19 mgrosso Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef KEYS_H
#define KEYS_H 1

// ----------------------------------------------------------------------
// BIG FAT WARNING
// BIG FAT WARNING
// BIG FAT WARNING
// 
// this file should just be preprocesser defines, nothing else, so that 
// it may be included from within c if need be.
// ----------------------------------------------------------------------
#define VARBUFLEN 8192

#define CPLUSQL_NAMESPACE  "NAMESPACE"
#define CPLUSQL_NAMESPACE_DFLT  "default"
#define CPLUSQL_USE_FILE_SYSTEM "file"

#define CPLUSQL_READ_BUFSIZE "CPLUSQL_READ_BUFSIZE"
#define CPLUSQL_READ_BUFSIZE_DFLT "262144"

#define CPLUSQL_MAX_BAD_PARSE "CPLUSQL_MAX_BAD_PARSE"
#define CPLUSQL_MAX_BAD_PARSE_DFLT "0"

#define CPLUSQL_DELIMITER "DELIMITER"
#define CPLUSQL_DELIMITER_DFLT "|"

#define CPLUSQL_NOTHREADS "CPLUSQL_NOTHREADS"
#define CPLUSQL_NOTHREADS_DFLT "0"


#define CPLUSQL_DEST_CLOSE_ON_DELETE "CPLUSQL_DEST_CLOSE_ON_DELETE"
#define CPLUSQL_DEST_O_EXCL "CPLUSQL_DEST_O_EXCL"
#define CPLUSQL_DEST_O_TRUNC "CPLUSQL_DEST_O_TRUNC"
#define CPLUSQL_CMD_PANIC_ON_FAILURE "PANIC_ON_FAILURE"

#define CPLUSQL_SKIP "CPLUSQL_SKIP"
#define CPLUSQL_SKIP_DFLT "0"
#define CPLUSQL_LIMIT  "CPLUSQL_LIMIT"
#define CPLUSQL_LIMIT_DFLT "0"

#define CPLUSQL_FORK "CPLUSQL_FORK"
#define CPLUSQL_FORK_DFLT "1" 
#define CPLUSQL_CHDIR "CPLUSQL_CHDIR"
#define CPLUSQL_CHDIR_DFLT "." 
#define CPLUSQL_PIDFILE "CPLUSQL_PIDFILE"
#define CPLUSQL_PIDFILE_DFLT ".cplusql.pid" 

#define SYNC_VERBOSE "SYNC_VERBOSE"
#define SYNC_VERBOSE_DFLT "0"


//GenericService parameters
#define NS_HOST "NS_HOST"
#define NS_PORT "NS_PORT"

#define GS_SERVICENAME "GS_SERVICENAME"
#define GS_INSTANCE_ID "GS_INSTANCE_ID"
#define GS_SERVICENAME "GS_SERVICENAME"
#define GS_HOSTNAME "GS_HOSTNAME"
#define GS_CORBANAME "GS_CORBANAME"
#define GS_MY_URL "GS_MY_URL"
#define GS_DEFAULT_REGISTER_TARGET_URL "GS_DEFAULT_REGISTER_TARGET_URL"
#define GS_DEFAULT_SUBSCRIBE_TARGET_URL "GS_DEFAULT_SUBSCRIBE_TARGET_URL"
#define GS_ROOTDIR "GS_ROOTDIR"
#define GS_FORK "GS_FORK"
#define GS_PIDFILE "GS_PIDFILE"

//NodeMonitor 
#define CNMS_NAME  "CentralNodeMonitorService"  
#define NMS_NAME  "NodeMonitorService"  
#define NMS_DEFAULT_SUBSCRIBER_URL "NMS_DEFAULT_SUBSCRIBER_URL"
#define NMS_DEFAULT_SUBSCRIBER_SERVICE_NAME "NMS_DEFAULT_SUBSCRIBER_SERVICE_NAME"
#define NMS_LIVENESS_THREAD_SLEEPTIME "NMS_LIVENESS_THREAD_SLEEPTIME"
#define NMS_LIVENESS_TOLERANCE "NMS_LIVENESS_TOLERANCE"
#define NMS_SUBSCRIBER_THREAD_SLEEPTIME "NMS_SUBSCRIBER_THREAD_SLEEPTIME"
#define NMS_REGISTRANT_THREAD_SLEEPTIME "NMS_REGISTRANT_THREAD_SLEEPTIME"

//JobExec configuration parameters
#define JE_MAX_SLOTS "JE_MAX_SLOTS"
#define JE_STATUS_DELIM "JE_STATUS_DELIM"
#define JE_MAX_BACKOFF "JE_MAX_BACKOFF"
#define JE_MAX_RETRY "JE_MAX_RETRY"
#define JE_BACKOFF_FACTOR "JE_BACKOFF_FACTOR"

//JobExec status parameters
#define JE_NAME "JobExecService"
#define JE_USED_SLOTS "JE_USED_SLOTS"
#define JE_AVAILABLE_SLOTS "JE_AVAILABLE_SLOTS"
#define JE_RUNNING_PIDS "JE_RUNNING_PIDS"
#define JE_RUNNING_JOBIDS "JE_RUNNING_JOBIDS"
#define JE_RUNNING_JOBTIMES "JE_RUNNING_JOBTIMES"

//per Job configuration parameters
#define JOB_EXCLUSIVE "JOB_EXCLUSIVE"
#define JOBQ_URL "JOB_URL"
#define JOB_TIMEOUT "JOB_TIMEOUT"
#define JOB_STARTDIR "JOB_STARTDIR"
#define JOB_ERROR "JOB_ERROR"
#define JOB_OUTPUT "JOB_OUTPUT"
#define JOB_TARGET "JOB_TARGET"
#define JOB_REQUIRES_HOST "JOB_REQUIRES_HOST"


//JobQueueService hard coded default name.
#define JQS_NAME "JobQueueService"
//JobQueueService configuration parameters
#define JQS_SCHEDULER_THREAD_SLEEPTIME "JQS_SCHEDULER_THREAD_SLEEPTIME"
#define JQS_ROOT "JQS_ROOT"
#define JQS_SUBMITQ_THR "JQS_SUMBMITQ_THR"
#define JQS_SUBMITQ_MAX "JQS_SUMBMITQ_MAX"
#define JQS_NODE_CNTL_DIR "JQS_NODE_CNTL_DIR"
#define JQS_OOS_SUFFIX "JQS_OOS_SUFFIX"
#define JQS_IOR_URI "JQS_IOR_URI"
#define JQS_IOR_FILE "JQS_IOR_FILE"
#define JQS_MIN_NODES "JQS_MIN_NODES"


//NameSpaceService hard coded default name.
#define NSS_NAME  "NameSpaceService"  
//NameSpaceService configuration parameters
#define NSS_NUMRMTHREADS  "NSS_NUMRMTHREADS"  
#define NSS_NUMCPTHREADS "NSS_NUMCPTHREADS" 
#define NSS_IORFILE "NSS_IORFILE" 
#define NSS_USE_FILE_SYSTEM "NSS_USE_FILE_SYSTEM"
#define NSS_RWS_MINFREE "NSS_RWS_MINFREE" 

//config parameters for nss clients
#define NSS_IOR_URI "NSS_IOR_URI" 
#define NSS_URL "NSS_URL" 
#define NSS_REPLICA_COUNT "NSS_REPLICA_COUNT" 

//NameSpaceService status keys
#define NSS_NUMFILES "NSS_NUMFILES" 
#define NSS_NUMNODES "NSS_NUMNODES" 


#define RWS_NAME "ReadWriteService"
//RWS status keys
#define RWS_INSTANCE_ID "RWS_INSTANCE_ID"
#define RWS_GB_DISK_FREE "RWS_GB_DISK_FREE"
//thoughts on these ?
//idea is to simply include number of appends since
//the last status pull, and zero counts with each
//pull, providing an idea of how busy nodes are.
#define RWS_APPEND_CNT "RWS_APPEND_CNT"
#define RWS_APPEND_BYTE_CNT "RWS_APPEND_CNT"
#define RWS_READ_CNT "RWS_READ_CNT"
#define RWS_READ_BYTE_CNT "RWS_READ_CNT"
#define RWS_MAX_CACHE_SIZE "RWS_MAX_CACHE_SIZE"
#define RWS_MAX_NOFILES "RWS_MAX_NOFIlES"

#endif /* KEYS_H */

