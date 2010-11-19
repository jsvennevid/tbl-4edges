///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "DebugServer.h"
#include <Shared/Base/Io/Log.h>
#include <kernel.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include <string.h>
#include <loadfile.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

DebugServer* DebugServer::m_instance = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DebugServer::DebugServer() : 
	m_exceedLimit(false)
{
	m_upBuffer = reinterpret_cast<CommandBuffer*>(&m_buffer1);
	m_currentBuffer = reinterpret_cast<CommandBuffer*>(&m_buffer2);
	m_upBuffer->commandCount = 0;
	m_currentBuffer->commandCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer::sifCallback(void* packet, void* /*param*/)
{
	CommandBuffer& upBuffer = DebugServer::instance().upBuffer();

	if (upBuffer.commandCount > MaxCommands)
	{
		DebugServer::instance().setExceedLimit(true);
		return;
	}

	//SifCmdHeader_t* header = static_cast<SifCmdHeader_t*>(packet);
	//u8* data = (u8*)packet;
	//data += sizeof(SifCmdHeader_t);	
	
	u16 commandCount = upBuffer.commandCount;
	
	upBuffer.commands[commandCount].size = 128;
	memcpy(upBuffer.commands[commandCount].buffer, packet, 128); 
	++upBuffer.commandCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool DebugServer::create()
{
	SifInitRpc(0);
	
	int ret = SifLoadModule("host:DebugServer.irx", 0, NULL);

	if (ret < 0)
	{
		ZENIC_INFO("Faild to load: " << ret);
		return false;
	}
	
	DIntr();
	SifAddCmdHandler(16, sifCallback, NULL);
	EIntr();
	
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void DebugServer::update()
{
	m_exceedLimit = false;

	m_currentBuffer->commandCount = 0;

	CommandBuffer* temp = m_currentBuffer;
	m_currentBuffer = m_upBuffer;
	m_upBuffer = temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
