///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Pad.h"
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <libpad.h>
#include <stdio.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ROM_PADMAN

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char Pad::s_padBuf[256] __attribute__((aligned(64)));
char Pad::s_actAlign[6];
int Pad::s_actuators;
bool Pad::s_modulesLoaded = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pad::loadModules()
{
	int ret;

#ifdef ROM_PADMAN
	ret = SifLoadModule("rom0:SIO2MAN", 0, 0);
#else
	ret = SifLoadModule("rom0:XSIO2MAN", 0, 0);
#endif

	if (ret < 0)
	{
		//Mandrake::Debug::Error("sifLoadModule sio failed: %d", ret);
		SleepThread();
	}

#ifdef ROM_PADMAN
	ret = SifLoadModule("rom0:PADMAN", 0, NULL);
#else
	ret = SifLoadModule("rom0:XPADMAN", 0, NULL);
#endif 

	if (ret < 0)
	{
		//Mandrake::Debug::Error("sifLoadModule pad failed: %d", ret);
		SleepThread();
	}

	padInit(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pad::Pad()
{
	m_port = 0;
	m_slot = 0;
	m_timeout = 50000;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pad::create(u32 port, u32 slot, u32 timeout)
{
	int ret;
	m_port = port;
	m_slot = slot;
	m_timeout = timeout;
	
	if (!s_modulesLoaded)
	{
		loadModules();
		s_modulesLoaded = true;
	}
		
	if ((ret = padPortOpen(m_port, m_slot, s_padBuf)) == 0)
	{
		//printf("padOpenPort failed: %d\n", ret);
		SleepThread();
	}
	
	m_oldPad = 0;
	
	init();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pad::update()
{
	int ret = padGetState(m_port, m_slot );

	while ((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1))
	{
		if (ret == PAD_STATE_DISCONN)
		{
			//printf("Pad(%d, %d) is disconnected\n", m_port, m_slot);
		}
		ret = padGetState(m_port, m_slot);
	}

	ret = padRead(m_port, m_slot, (padButtonStatus*)&m_buttonStatus); // port, slot, buttons

	if (ret != 0)
	{
		m_padData = 0xffff ^ m_buttonStatus.btns;

		m_newPad = m_padData & ~m_oldPad;
		m_oldPad = m_padData;

		memset(&m_buttons, 0, sizeof(Buttons));

		if (m_newPad & PAD_LEFT) 
			m_buttons.left = true;

		if (m_newPad & PAD_DOWN) 
			m_buttons.down = true;

		if (m_newPad & PAD_RIGHT) 
			m_buttons.rigth = true;

		if (m_newPad & PAD_UP) 
			m_buttons.up = true;

		if (m_newPad & PAD_START) 
			m_buttons.start = true;
		if (m_newPad & PAD_SELECT) 
			m_buttons.select = true;

		if (m_newPad & PAD_SQUARE) 
			m_buttons.square = true;

		if (m_newPad & PAD_CROSS) 
			m_buttons.cross = true;

		if (m_newPad & PAD_CIRCLE) 
			m_buttons.circle = true;

		if (m_newPad & PAD_TRIANGLE) 
			m_buttons.triangle = true;

		if (m_newPad & PAD_R1) 
			m_buttons.r1 = true;

		if (m_newPad & PAD_L1) 
			m_buttons.l1 = true;

		if (m_newPad & PAD_R2) 
			m_buttons.r2 = true;

		if (m_newPad & PAD_L2) 
			m_buttons.l2 = true;

		const int deadRange = 30;
		const int deadMin = 127 - deadRange;
		const int deadMax = 128 + deadRange;

		int test = 0;

		if (m_buttonStatus.rjoy_h < deadMin)
		{
			m_rStickH = (m_buttonStatus.rjoy_h - 127) / 127.0f; 
		}
		else if (m_buttonStatus.rjoy_h > deadMax)
		{
			m_rStickH = (m_buttonStatus.rjoy_h - 127) / 127.0f; 
		}
		else
		{
			m_buttonStatus.rjoy_h = 0;
			m_rStickH = 0.0f;
		}

		if (m_buttonStatus.rjoy_v < deadMin)
		{
			m_rStickV = (m_buttonStatus.rjoy_v - 127) / 127.0f; 
		}
		else if (m_buttonStatus.rjoy_v > deadMax)
		{
			test = m_buttonStatus.rjoy_v - 127;
			m_rStickV = (m_buttonStatus.rjoy_v - 127) / 127.0f; 
		}
		else
		{
			m_buttonStatus.rjoy_v = 0;
			m_rStickV = 0.0f;
		}

		if (m_buttonStatus.ljoy_h < deadMin)
		{
			m_lStickH = (m_buttonStatus.ljoy_h - 127) / 127.0f; 
		}
		else if (m_buttonStatus.ljoy_h > deadMax)
		{
			m_lStickH = (m_buttonStatus.ljoy_h - 127) / 127.0f; 
		}
		else
		{
			m_buttonStatus.ljoy_h = 0;
			m_lStickH = 0.0f;
		}

		if (m_buttonStatus.ljoy_v < deadMin)
		{
			m_lStickV = (m_buttonStatus.ljoy_v - 127) / 127.0f; 
		}
		else if (m_buttonStatus.ljoy_v > deadMax)
		{
			test = m_buttonStatus.ljoy_v - 127;
			m_lStickV = (m_buttonStatus.ljoy_v - 127) / 127.0f; 
		}
		else
		{
			m_buttonStatus.ljoy_v = 0;
			m_lStickV = 0.0f;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Pad::init()
{
	int ret;
	int modes;
	int i;

	waitReady();

	// How many different modes can this device operate in?
	// i.e. get # entrys in the modetable

	modes = padInfoMode(m_port, m_slot, PAD_MODETABLE, -1);
	//printf("The device has %d modes\n", modes);

	if (modes > 0)
	{
		//printf("( ");
		for (i = 0; i < modes; i++)
		{
			//printf("%d ", padInfoMode(m_port, m_slot, PAD_MODETABLE, i));
		}
		//printf(")");
	}

	//printf("It is currently using mode %d\n", padInfoMode(m_port, m_slot, PAD_MODECURID, 0));

	// If modes == 0, this is not a Dual shock controller 
	// (it has no actuator engines)

	if (modes == 0)
	{
		//printf("This is a digital controller?\n");
		return false;
	}

	// Verify that the controller has a DUAL SHOCK mode
	i = 0;
	do
	{
		if (padInfoMode( m_port, m_slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
			break;
		i++;
	} while (i < modes);

	if (i >= modes)
	{
		//printf("This is no Dual Shock controller\n");
		return false;
	}

	// If ExId != 0x0 => This controller has actuator engines
	// This check should always pass if the Dual Shock test above passed

	ret = padInfoMode(m_port, m_slot, PAD_MODECUREXID, 0);
	if (ret == 0)
	{
		//printf("This is no Dual Shock controller??\n");
		return false;
	}

	//printf("Enabling dual shock functions\n");

	// When using MMODE_LOCK, user cant change mode with Select button
	padSetMainMode(m_port, m_slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

	waitReady();
	//printf("infoPressMode: %d\n", padInfoPressMode(m_port, m_slot));

	waitReady();
	//printf("enterPressMode: %d\n", padEnterPressMode(m_port, m_slot));

	waitReady();
	s_actuators = padInfoAct(m_port, m_slot, -1, 0);
	//printf("# of m_actuators: %d\n",ms_actuators);

	if (s_actuators != 0)
	{
		s_actAlign[0] = 0;	 // Enable small engine
		s_actAlign[1] = 1;	 // Enable big engine
		s_actAlign[2] = 0xff;
		s_actAlign[3] = 0xff;
		s_actAlign[4] = 0xff;
		s_actAlign[5] = 0xff;

		waitReady();
		//printf("padSetActAlign: %d\n", 
		//padSetActAlign(m_port, m_slot, ms_actAlign));
	}
	else
	{
		//printf("Did not find any m_actuators.\n");
	}

	waitReady();
	update();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Pad::waitReady()
{
	int state;
	char stateString[16];

	state = padGetState(m_port, m_slot);
	int lastState = -1;

	while ((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1))
	{
		if (state != lastState)
		{
			padStateInt2String(state, stateString);
			//printf("Please wait, pad(%d,%d) is in state %s\n",m_port, m_slot, stateString);
		}

		lastState = state;
		state = padGetState(m_port, m_slot);
	}

	// Were the pad ever 'out of sync'?

	if (lastState != -1)
	{
		//printf("Pad OK!\n");
	}

	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

