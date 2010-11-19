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

#ifndef zenic_ps2_MountainsPart_h
#define zenic_ps2_MountainsPart_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Part.h"
#include <Shared/Base/Types.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/FadeToFog.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/DepthOfField.h>
#include <Shared/Graphics/Renderer/Ps2/RenderSettings.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class SceneInfo;

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

class MountainsPart : public Part
{

public:

	MountainsPart(Renderer& renderer, Dma& dmaChain);
	virtual ~MountainsPart() {}

	bool create();
	void update(float time, float deltaTime, SyncPoints::Part* syncPoints = 0);
	void reset();

private:

	Texture* loadTexture(const char* filename);
	void showCredits(float time, float deltaTime);
	void showImage4(const Texture* texture, float strength, float deltaTime);
	void showImage8(const Texture* texture, float strength, float deltaTime);

	DepthOfField m_depthOfFieldPostEffect;

	enum
	{
		LineCount = 5,
		CubesPerLineCount = 100,
	};

	struct CubeInstance
	{
		Matrix4 matrix;
		Vector3 pos;
		float angleX, angleY, angleZ;
		float rotSpeedX, rotSpeedY, rotSpeedZ;
		float scale;
		float shadowOffset;
		float speed;
	};

	CubeInstance* m_lines[LineCount];
	float m_lineSpeed[LineCount];

	SceneInfo* m_scene;
	Renderer& m_renderer;
	Dma& m_dmaChain;

	Texture* m_credits[15];
	float m_tempTime;
	uint m_currentText;

	float m_textPosY;
	float m_textPosX;
	float m_textAdd;

	uint m_syncPos;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline MountainsPart::MountainsPart(Renderer& renderer, Dma& dmaChain) :
	m_depthOfFieldPostEffect(RenderSettings()),
	m_renderer(renderer),
	m_dmaChain(dmaChain)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



