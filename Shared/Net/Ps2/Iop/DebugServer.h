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

#ifndef corona_DebugServer_h
#define corona_DebugServer_h

#define MAX_CONNECTIONS 5

typedef struct st_DebugServerData
{
  int mainSocket;
  int highSocket;
  int connections[MAX_CONNECTIONS];

} DebugServerData;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int DebugServer_create(DebugServerData* data);
void DebugServer_handleNewConnection(DebugServerData* data);
void DebugServer_update(DebugServerData* data);
void DebugServer_handleData(DebugServerData* data, int id);
void DebugServer_destroy(DebugServerData* data);

#define MDSERVER_BIND_RPC_ID 0x123C001
#define MDSERVER_READ 0x1

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif

