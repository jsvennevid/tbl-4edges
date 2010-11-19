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

#ifndef zenic_ps2_TunnelPart_h
#define zenic_ps2_TunnelPart_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Part.h"
#include <Shared/Base/Types.h>
#include <Shared/Base/Storage/String.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class SceneInfo;
	class Model;

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

class TunnelPart : public Part
{

public:

	TunnelPart(Renderer& renderer, Dma& dmaChain);
	virtual ~TunnelPart() {}

	bool create();
	void update(float time, float deltaTime, SyncPoints::Part* syncPoints = 0);
	void reset() {};

private:

	Renderer& m_renderer;
	Dma& m_dmaChain;
	SceneInfo* m_scene;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline TunnelPart::TunnelPart(Renderer& renderer, Dma& dmaChain) :
	m_renderer(renderer),
	m_dmaChain(dmaChain)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif




