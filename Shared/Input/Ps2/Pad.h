/*

Copyright (c) 2004-2006 Jesper Svennevid, Daniel Collin

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

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
	} ButtonStatus ZENIC_PACKED;

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

	static char s_padBuf[256] ZENIC_ALIGN(64);
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
