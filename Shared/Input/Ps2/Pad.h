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

#ifndef zenic_ps2_Pad
#define zenic_ps2_Pad

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Pad
{
public:

	typedef struct st_ButtonStatus
	{
		u8 ok;
		u8 mode;
		u16 btns;
		u8 rjoy_h;
		u8 rjoy_v;
		u8 ljoy_h;
		u8 ljoy_v;
		u8 rigth;
		u8 left;
		u8 up;
		u8 down;
		u8 triangle;
		u8 circle;
		u8 square;
		u8 cross;
		u8 l1;
		u8 r1;
		u8 l2;
		u8 r2;
		u8 unkn16[12];
	} ButtonStatus __attribute__((packed));

	struct Buttons
	{
		u32 left:1;
		u32 down:1;
		u32 rigth:1;
		u32 up:1;
		u32 start:1;
		u32 select:1;
		u32 triangle:1;
		u32 circle:1;
		u32 square:1;
		u32 cross:1;
		u32 l1:1;
		u32 r1:1;
		u32 l2:1;
		u32 r2:1;
	};

	Pad();
	void create(u32 port, u32 slot, u32 timeout = 50000);
	void update();

	static void loadModules();

	const Buttons& status() const;
	float rstickH();
	float rstickV();
	float lstickH();
	float lstickV();

private:

	bool init();
	bool waitReady();	

	ButtonStatus m_buttonStatus;
	Buttons m_buttons;

	float m_rStickH;
	float m_rStickV;
	float m_lStickH;
	float m_lStickV;
	
	u32 m_padData;
	u32 m_oldPad;
	u32 m_newPad;
	u32 m_port;
	u32 m_slot;
	u32 m_timeout;

	static char s_padBuf[256] __attribute__((aligned(64)));
	static char s_actAlign[6];
	static int s_actuators;
	static bool s_modulesLoaded;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Pad::Buttons& Pad::status() const
{ 
	return m_buttons; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Pad::rstickH() 
{ 
	return m_rStickH; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Pad::rstickV() 
{ 
	return m_rStickV; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Pad::lstickH() 
{ 
	return m_lStickH; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float Pad::lstickV() 
{ 
	return m_lStickV; 
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
