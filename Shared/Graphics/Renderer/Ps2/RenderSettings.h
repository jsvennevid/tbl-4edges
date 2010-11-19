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

#ifndef zenic_ps2_RenderSettings_h
#define zenic_ps2_RenderSettings_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class RenderSettings
{
public:

	RenderSettings(u16 screenWidth = 512, u16 screenHeight = 448, u16 textureWidth = 9, u16 textureHeight = 9, 
				   u16 screenFbw = 8, u16 buffer0 = 0, u16 buffer1 = 112, u16 zbuffer = 224, 
				   u16 tempBuffer0 = 336, u16 tempBuffer1 = 280);

	u16 screenWidth() const;
	u16 screenHeight() const;

	u16 textureWidth() const;
	u16 textureHeight() const;
	
	u16 screenFbw() const;

	u16 buffer0() const;
	u16 buffer1() const;
	u16 zbuffer() const;

	u16 tempBuffer0() const;
	u16 tempBuffer1() const;

private:

	u16 m_screenWidth;
	u16 m_screenHeight;

	u16 m_textureWidth;
	u16 m_textureHeight;

	u16 m_screenFbw;

	u16 m_buffer0;
	u16 m_buffer1;
	u16 m_zbuffer;

	u16 m_tempBuffer0;
	u16 m_tempBuffer1;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline RenderSettings::RenderSettings(u16 screenWidth, u16 screenHeight, u16 textureWidth, u16 textureHeight, 
									  u16 screenFbw, u16 buffer0, u16 buffer1, u16 zbuffer, 
									  u16 tempBuffer0, u16 tempBuffer1) :
	m_screenWidth(screenWidth),
	m_screenHeight(screenHeight),
	m_textureWidth(textureWidth),
	m_textureHeight(textureHeight),
	m_screenFbw(screenFbw),
	m_buffer0(buffer0),
	m_buffer1(buffer1),
	m_zbuffer(zbuffer),
	m_tempBuffer0(tempBuffer0),
	m_tempBuffer1(tempBuffer1)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::screenWidth() const
{
	return m_screenWidth;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::screenHeight() const
{
	return m_screenHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline	u16 RenderSettings::textureWidth() const
{
	return m_textureWidth;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline	u16 RenderSettings::textureHeight() const
{
	return m_textureHeight;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline	u16 RenderSettings::screenFbw() const
{
	return m_screenFbw;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::buffer0() const
{
	return m_buffer0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::buffer1() const
{
	return m_buffer1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::zbuffer() const
{
	return m_zbuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::tempBuffer0() const
{
	return m_tempBuffer0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u16 RenderSettings::tempBuffer1() const
{
	return m_tempBuffer1;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif


