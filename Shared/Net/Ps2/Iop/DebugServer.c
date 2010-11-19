///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DebugServer.h"
#include "irx_imports.h"

#define S_TRACE()

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define DataSize 2050 // 2 extra bytes for size

DebugServerData g_serverData;
static char sendBuffer[DataSize + 16];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int debugServer(void* data)
{
	printf("DebugServer Created\n");

	while (1)
	{
		DebugServer_update(&g_serverData);
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 _start(int argc, char* argv[])
{
	iop_thread_t mythread;
	int pid;
	int i;

	S_TRACE()

	if (!DebugServer_create(&g_serverData))
		return MODULE_NO_RESIDENT_END;

	// Start socket server thread

	mythread.attr = 0x02000000;	// attr
	mythread.option = 0; // option
	mythread.thread = (void *)debugServer; // entry
	mythread.stacksize = 0x1000;
	mythread.priority = 0x43; // just above ps2link

	pid = CreateThread(&mythread);

	if (pid > 0)
	{
		if ((i=StartThread(pid, NULL)) < 0)
		{
			DebugServer_destroy(&g_serverData);
			printf("mdserver: StartThread failed (%d)\n", i);
			return MODULE_NO_RESIDENT_END;
		}
	} 
	else
	{
		DebugServer_destroy(&g_serverData);
		printf("mdserver: CreateThread failed (%d)\n", pid);
		return MODULE_NO_RESIDENT_END;
	}

	return MODULE_RESIDENT_END;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int DebugServer_create(DebugServerData* data)
{
	s32 ret;
	int opt = 1;
	struct sockaddr_in listenerAddr;
	s32 listenerHostPort = 1337;

	memset(data, 0, sizeof(DebugServerData));

	S_TRACE()

	memset(&listenerAddr, 0, sizeof(listenerAddr));
	listenerAddr.sin_family = AF_INET;
	listenerAddr.sin_port = htons(listenerHostPort);
	listenerAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	S_TRACE()

	if ((data->mainSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("socket: failed to create.\n");
		return FALSE;
	}

	S_TRACE()

	if (setsockopt(data->mainSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		disconnect(data->mainSocket);
		printf("mdserver: Could not change socket options.\n");
		return -1;
	}

	S_TRACE()

	if ((ret = bind(data->mainSocket, (struct sockaddr*)&listenerAddr, sizeof(struct sockaddr))) < 0)
	{
		printf("bind: failed to bind.\n");
		return FALSE;
	}

	S_TRACE()

	if ((ret = listen(data->mainSocket, MAX_CONNECTIONS)) < 0)
	{
		printf("listen: failed to listen.");
		return FALSE;
	}

	data->highSocket = data->mainSocket+1;

	S_TRACE()

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer_update(DebugServerData* data)
{
	fd_set socks; 
	u32 i;
	int rv;

	FD_ZERO(&socks);
	FD_SET(data->mainSocket,&socks);

	for (i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (data->connections[i] != 0)
		{
			FD_SET(data->connections[i], &socks);
			if (data->connections[i] > data->highSocket)
				data->highSocket = data->connections[i];
		}
	}

	rv = select(data->highSocket+1, &socks, NULL, NULL, NULL);

	if (rv > 0)
	{
		if (FD_ISSET(data->mainSocket, &socks))
			DebugServer_handleNewConnection(data);

		for (i = 0; i < MAX_CONNECTIONS; ++i)
		{
			if (FD_ISSET(data->connections[i],& socks))
				DebugServer_handleData(data,i);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer_handleNewConnection(DebugServerData* data)
{
	struct sockaddr_in echoClntAddr;
	int clntLen = sizeof(echoClntAddr);
	u32 i;

	int connection = (int)accept(data->mainSocket, (struct sockaddr *)&echoClntAddr, &clntLen);
	if (connection < 0)
	{
		printf("accept: failed.\n");
		return;
	}

	for (i = 0; (i < MAX_CONNECTIONS) && (connection != -1); ++i)
	{
		if (data->connections[i] == 0)
		{
			printf("Connection accepted: FD=%d Slot:%d\n", (int)connection, (int)i);
			data->connections[i] = connection;
			connection = -1;
		}
	}

	if (connection != -1)
	{
		printf("No room left for new client.\n");
		disconnect(connection);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer_handleData(DebugServerData* data, int id)
{
	static char buffer[DataSize];    
	int i;

	// can be removed

	for (i = 0; i < DataSize; ++i)
		buffer[i] = 0;

	int rec = recv(data->connections[id], buffer, 2048, 0);

	if (rec < 0)
	{
		printf("Connection lost: FD=%d Slot: %d\n", data->connections[id], id);
		disconnect(data->connections[id]);
		data->connections[id] = 0;
		return;
	}

	for (i = 0; i < rec; ++i)
		sendBuffer[i+16] = buffer[i];

	isceSifSendCmd(16, sendBuffer, 16 + 16, NULL, NULL, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer_destroy(DebugServerData* data)
{
	u32 i;

	for (i = 0; i < MAX_CONNECTIONS; ++i)
	{
		if (data->connections[i] != 0)
			disconnect(data->connections[i]);
	} 
}

