// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
//
// File:      netwrap.h
// Author:    sjackson Steve Jackson
// Created:   
// Project:   
// Purpose:   simple classes to avoid typing the same thing every time
//          I open a file.
// 
// Copyright (c) 2004 LookSmart Inc.  All Rights Reserved.
// 
// $Id: netwrap.h,v 1.3 2004/06/30 19:02:28 sjackson Exp $
// ----------------------------------------------------------------------
// ----------------------------------------------------------------------
#ifndef NETWRAP_H
#define NETWRAP_H 1

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>


class netwrap 
{
public:
	static long long htonll(long long l);

	// Return a socket suitable for listen()ing on.  Return -1 if socket()
	// creation failed, -2 if bind() failed.  Specify host if you want to
	// bind to particular NIC; failure will cause -3 to be returned.
	//
	static int createServer(int &port, const char *host = (const char *) 0);

	// Return a new socket that a client is hooked up to.  Return -1 if
	// listen() failed, -2 if accept() failed.
	//
	static int waitForClient(int serverSocket, struct sockaddr *addr = (struct sockaddr *) 0, int backlog = 5);

	// Connect to a server.  Host can be either an IP address or a hostname.
	// Return the socket.  Return -1 if no such host/IP, -2 if socket()
	// creation failed, -3 if connect() failed.
	//
	static int connectToServer(const char *host, int port);

	// Return 0 on success.
	//
	static int closeServer(int sock);
	static int closeClient(int sock);

	// Stores the IP addr in ipaddr.  Returns: 0 on success, -1 for invalid
	// args, -2 for no such host, -3 for no such interface.
	//
	static int getIpAddr(const char *host, in_addr_t *ipaddr, int whichInterface = 0);

};


#endif /* NETWRAP_H */
