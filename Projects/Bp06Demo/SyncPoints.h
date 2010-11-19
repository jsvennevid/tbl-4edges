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

#ifndef zenic_ps2_SyncPoints_h
#define zenic_ps2_SyncPoints_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class MusicStream;

	namespace ps2
	{
		class Pad;
		class Texture;
		class Dma;
		class Renderer;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SyncPoints
{

public:

	enum Type
	{
		Custom,	// one float value for custom types
		CameraChange
	};

	struct CustomType
	{
		float value;
	};

	struct SwitchCamera
	{
		float time;
		u32 cameraId;
	};

	struct SyncPoint
	{
		Type type;
		u8 data[256];	// Can be any kind of data, depends on type.
	};

	struct Part
	{
		~Part() { syncPoints.clear(); }

		uint id;
		float startTime;

		Array<SyncPoint> syncPoints;	
	};

	SyncPoints();
	void reloadFile();
	float update(Pad& pad, MusicStream& stream, float time);
	float update(float time);
	float partStartTime();
	uint partId();

private:

	void parseFile(char* buffer);

	Array<Part*> m_parts;

	int m_currentPart;
	uint m_currentSyncPoint;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



