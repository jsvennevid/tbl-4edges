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

#ifndef zenic_ps2_SaPart_h
#define zenic_ps2_SaPart_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Part.h"
#include <Shared/Base/Types.h>
#include <Shared/Base/Storage/String.h>
#include <Shared/Geometry/Ps2/Texture.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class SceneInfo;
	class Model;
	class Matrix4;

	namespace ps2
	{
		class Texture;
		class Dma;
		class Renderer;
		class PadCamera;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SaPart : public Part
{

public:

	SaPart(Renderer& renderer, Dma& dmaChain);

	static void loadSaObjects();
	static Model* loadObject(const char* filename); 
	void addDisolveObject(const u8* fftData, uint objectId, const Matrix4& matrix, uint addOffset = 0, uint mulValue = 255);

	virtual bool create() { return true; };
	virtual void reset() {};
	virtual void update(float time, float deltaTime, SyncPoints::Part* syncPoints = 0) {};

	SceneInfo* scene() const;
	void setScene(SceneInfo* scene);

	enum
	{
		MaxSaObjects = 7
	};

protected:

	void loadScene(const char* filename);

	static void updateTexture(Texture* texture, const u8* fftData, uint addOffset = 0, uint mulValue = 255);
	static void initTexture(Texture& texture);

	Renderer& m_renderer;
	Dma& m_dmaChain;
	SceneInfo* m_scene;

	static Model* s_objects[MaxSaObjects];
	static Texture s_textures[MaxSaObjects];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SaPart::SaPart(Renderer& renderer, Dma& dmaChain) : 
	m_renderer(renderer), 
	m_dmaChain(dmaChain),
	m_scene(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline SceneInfo* SaPart::scene() const
{
	return m_scene;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void SaPart::setScene(SceneInfo* scene)
{
	m_scene = scene;
}
 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



