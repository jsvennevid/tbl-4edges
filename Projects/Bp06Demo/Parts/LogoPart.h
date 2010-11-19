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

#ifndef zenic_ps2_LogoPart_h
#define zenic_ps2_LogoPart_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Part.h"
#include <Shared/Base/Types.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/FadeToFog.h>
#include <Shared/Graphics/Renderer/Ps2/RenderSettings.h>
#include <Shared/Graphics/Scene/Camera.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Camera;

	namespace ps2
	{
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

class LogoPart : public Part
{

public:

	enum
	{
		LogoX = 64,
		LogoY = 64,
		LogoSize = LogoX * LogoY
	};

	LogoPart(Renderer& renderer, Dma& dmaChain);
	virtual ~LogoPart() {}

	bool create();
	void reset() {};
	bool destroy();
	void update(float time, float deltaTime, SyncPoints::Part* syncPoints = 0);

private:

	struct Cube
	{
		Vector3 pos;
		Vector3 direction;
		Vector3 rot;
		Vector3 rotSpeed;
		float speed;
		float gravity;
	};

	u8 m_logo[LogoSize];
	zenic::Camera m_sceneCamera;
	Renderer& m_renderer;
	Dma& m_dmaChain;
	Cube* m_cubes;
	uint m_cubeCount;
	float m_zPos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline LogoPart::LogoPart(Renderer& renderer, Dma& dmaChain) :
	m_renderer(renderer),
	m_dmaChain(dmaChain),
	m_cubes(0),
	m_cubeCount(0),
	m_zPos(420.0f)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif




