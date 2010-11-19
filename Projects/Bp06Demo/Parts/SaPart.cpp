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

#include "SaPart.h"
#include <Shared/Base/Io/FileSystem.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Geometry/Geometry.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/CubeRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/ShRenderer.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>
#include <Shared/Base/Io/Log.h>
#include "../FftStream.h"
#include "../PadCamera.h"

#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

Model* SaPart::s_objects[SaPart::MaxSaObjects];
Texture SaPart::s_textures[MaxSaObjects];


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart::loadSaObjects()
{
	s_objects[0] = loadObject("DATA/OUTPUT/SAMODEL1.ZEO");
	s_objects[1] = loadObject("DATA/OUTPUT/SAMODEL4.ZEO");
	s_objects[2] = loadObject("DATA/OUTPUT/SSPECTRUM.ZEO");
	s_objects[3] = loadObject("DATA/OUTPUT/SAMODEL2.ZEO");
	s_objects[4] = loadObject("DATA/OUTPUT/SAMODEL3.ZEO");
	s_objects[5] = loadObject("DATA/OUTPUT/SAMODEL5.ZEO");
	s_objects[6] = loadObject("DATA/OUTPUT/SAMODEL6.ZEO");

	initTexture(s_textures[0]);
	initTexture(s_textures[1]);
	initTexture(s_textures[2]);
	initTexture(s_textures[3]);
	initTexture(s_textures[4]);
	initTexture(s_textures[5]);
	initTexture(s_textures[6]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Model* SaPart::loadObject(const char* filename)
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, filename))
		return false;

	if (!inBinary.load(stream))
		return false;

	return static_cast<Model*>(inBinary[0]);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart::loadScene(const char* filename)
{
	FileSystem& fileSystem = FileSystem::instance();
	InBinarySerializer inBinary;

	FileStream stream;

	if (!fileSystem.open(stream, filename))
		return;

	if (!inBinary.load(stream))
		return;

	m_scene = static_cast<SceneInfo*>(inBinary[0]);
	m_renderer.initScene(m_scene);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart::initTexture(Texture& saTexture)
{
	// Create a 32x32 texture that is used for spectrum analyzer objects

	Texture::Parameters uploadParameters(1, gs::T8, 0, 32, 32);
	Texture::Parameters displayParameters(1, gs::T8, 0, 5, 5);

	DataPtr<u8> pal;
	DataPtr<u8> texture;

	pal.allocate(256 * 4);
	u32* palObjects = (u32*)pal.objects();

	texture.allocate(32 * 32);
	u8* textureData = texture.objects();

	ZENIC_ASSERT(palObjects);
	ZENIC_ASSERT(textureData);

	//u32 tempPal[256];

	for (u32 p = 0;  p < 256; ++p)
		palObjects[(p & 231) + ((p & 8) << 1) + ((p & 16) >> 1)] = (p << 24) | (p << 16) | (p << 8) | p;


	for (uint i = 0; i < 32; ++i)
	{
		for (uint k = 0; k < 32; ++k)
			textureData[k] = i << 3;

		textureData += 32;
	}

	saTexture.setUploadParameters(uploadParameters);
	saTexture.setDisplayParameters(displayParameters);
	saTexture.setMipMaps(1);
	saTexture.setPalette(pal);
	saTexture.setData(0, texture);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart::updateTexture(Texture* texture, const u8* fftData, uint addOffset, uint mulValue)
{
	u8* textureData = texture->map(0).objects();

	for (uint y = 0; y < 32; ++y)
	{
		uint color = ((fftData[y >> 1])) * (rand() % mulValue);

		if (color > 255)
			color = 255;

		//color = rand() & 0xff;

		for (uint x = 0; x < 32; ++x)
			textureData[x] = color;

		textureData += 32;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SaPart::addDisolveObject(const u8* fftData, uint objectId, const Matrix4& matrix, uint addOffset, uint mulValue)
{
	ZENIC_ASSERT(objectId < MaxSaObjects);
	updateTexture(&s_textures[objectId], fftData, addOffset, mulValue);
	m_renderer.addAlphaDissolveModel(&s_textures[objectId], (const Model*)s_objects[objectId], matrix);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}




