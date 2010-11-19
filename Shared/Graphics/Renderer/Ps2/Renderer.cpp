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

#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/Material.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <Shared/Hardware/Ps2/MasterChain.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/DmaSimulator.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Geometry/Ps2/ModelData.h>
#include <Shared/Geometry/Ps2/TempTexture.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Base/Memory/Prefetch.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/GlenzNode.h>
#include <Shared/Graphics/Renderer/Ps2/Shaders/PostEffect/PostEffectHelper.h>

#ifndef PS2_EMU
#include <kernel.h>
#include <screenshot.h>
#endif

#include <stdio.h>
#include <string.h>
#include "AssertDisplay.h"
#include "CubeRenderer.h"
#include "ShRenderer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PS2_EMU
extern "C" u32 IntTexturedNonShaded_CodeStart __attribute__((section(".vudata")));
extern "C" u32 IntTexturedNonShaded_CodeEnd __attribute__((section(".vudata")));
extern "C" u32 FloatTexturedNonShaded_CodeStart __attribute__((section(".vudata")));
extern "C" u32 FloatTexturedNonShaded_CodeEnd __attribute__((section(".vudata")));
extern "C" u32 IntFlatNonShaded_CodeStart __attribute__((section(".vudata")));
extern "C" u32 IntFlatNonShaded_CodeEnd __attribute__((section(".vudata")));
extern "C" u32 FloatFlatNonShaded_CodeStart __attribute__((section(".vudata")));
extern "C" u32 FloatFlatNonShaded_CodeEnd __attribute__((section(".vudata")));
extern "C" u32 AlphaDissolveEnvmapped_CodeStart __attribute__((section(".vudata")));
extern "C" u32 AlphaDissolveEnvmapped_CodeEnd __attribute__((section(".vudata")));
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

static Texture textures[4];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer(bool renderFrames) : 
	m_textureSystem(TextureSystem::Path2),
	m_sceneCamera(0), 
	m_frameCount(0),
	m_disableOrgGlowObjects(false),
	m_flip(false), 
	m_renderFrames(renderFrames),
	m_currentField(false),
	m_screenShot(false),
	m_alphaDisolveCount(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


Renderer::~Renderer()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::create()
{
#ifdef PS2_EMU
	m_emuDisplay.create(512, 448);
#endif

#if !defined(ZENIC_FINAL) && !defined(PS2_EMU)
	Assert::setCallback(AssertDisplay::callback);
#endif
	Vu1::setInstance(zenic_new Vu1);

	m_width = 512;
	m_height = 448;
	m_psm = gs::CT32;
	bool deflicker = true;

	switch (m_psm)
	{
		case gs::CT24: 
		case gs::CT32:
		{
			m_frontBuffer = 0 * (m_width / 64) * (m_height / 32);
			m_backBuffer = 1 * (m_width / 64) * (m_height / 32);
			m_zbuffer = 2 * (m_width / 64) * (m_height / 32);
			m_targetBuffer = 3 * (m_width / 64) * (m_height / 32);
		}
		break;

		case gs::CT16:
		{
			m_frontBuffer = 0 * (m_width / 64) * (m_height / 64);
			m_backBuffer = 1 * (m_width / 64) * (m_height / 64);
			m_zbuffer = 2 * (m_width / 64) * (m_height / 64);
			m_targetBuffer = 3 * (m_width / 64) * (m_height / 64);
		}
		break;
	
		default:
		break;
	}

#ifdef ZENIC_PS2

	//Setup DispFb
	GS_WRITE(DISPFB_1,GS_DISPFB(m_frontBuffer, m_width / 64, m_psm, 0, deflicker));
	GS_WRITE(DISPFB_2,GS_DISPFB(m_frontBuffer, m_width / 64, m_psm, 0, 0));

	//Setup readCircts
	GS_WRITE(DISPLAY_1, makeDisplayReg());
	GS_WRITE(DISPLAY_2, makeDisplayReg());

	//Setup circt combinder
	GS_WRITE(PMODE,GS_PMODE(deflicker,true,true,true,false,0x7f));

	SetGsCrt(1, 0, 0);

	//interlace and FILED mode
	GS_WRITE(SMODE_2,GS_SMODE2(true, 0, 0));
#endif
	//setup backbuffer enviroment

	m_backBufferEnv.initHeader();
	m_backBufferEnv.setDefault();

	m_backBufferEnv.setXyOffset((2048 - m_width / 2) * 16, (2048 - m_height / 2) * 16);
	MasterChain::setInstance(new MasterChain(1000 * 1024));

	// Create a bunch of temporary textures

	m_textureSystem.create();

#ifdef ZENIC_PS2
	*GIF_MODE = 0;
#endif

	// Create a 32x32 texture that is used for spectrum analyzer objects

	Texture::Parameters uploadParameters(1, gs::T8, 0, 32, 32);
	Texture::Parameters displayParameters(1, gs::T8, 0, 5, 5);

	DataPtr<u8> pal;
	DataPtr<u8> texture;

	pal.allocate(256 * 4);
	u32* palObjects = (u32*)pal.objects();

	texture.allocate(32 * 32);
	u8* textureData = texture.objects();

	//u32 tempPal[256];

	for (u32 p = 0;  p < 256; ++p)
		palObjects[(p & 231) + ((p & 8) << 1) + ((p & 16) >> 1)] = (p << 24) | (p << 16) | (p << 8) | p;


	for (uint i = 0; i < 32; ++i)
	{
		for (uint k = 0; k < 32; ++k)
			textureData[k] = 0x40;

		textureData += 32;
	}

	m_spectrumTexture.setUploadParameters(uploadParameters);
	m_spectrumTexture.setDisplayParameters(displayParameters);
	m_spectrumTexture.setMipMaps(1);
	m_spectrumTexture.setPalette(pal);
	m_spectrumTexture.setData(0, texture);

	addClear();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::init()
{
	m_cubeRenderer = zenic_new CubeRenderer(*this, MasterChain::instance()->chain());
	m_shRenderer = zenic_new ShRenderer(*this, MasterChain::instance()->chain());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::destroy()
{
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::flip()
{
	#define CSR ((volatile u64 *)(0x12001000))

	u32 temp = m_frontBuffer;
	m_frontBuffer = m_backBuffer;
	m_backBuffer = temp;

	Dma& masterChain = MasterChain::instance()->chain();
	MasterChain::instance()->wait();
	m_textureSystem.wait();


#ifdef PS2_EMU
	m_emuDisplay.update();
#endif

	//vsync

#ifdef ZENIC_PS2

	GS_WRITE(BGCOLOR,GS_BGCOLOR(255,255,0));

	*CSR = *CSR & 8;
	while(!(*CSR & 8))
	{
		//idle to not hammer on the buss too much.
		asm __volatile__( "nop;nop;nop;nop;nop;nop;nop;nop;" );
	}


#if 1
	GS_WRITE(DISPFB_1,GS_DISPFB(m_frontBuffer, m_width / 64, m_psm, 0, 1));
	GS_WRITE(DISPFB_2,GS_DISPFB(m_frontBuffer, m_width / 64, m_psm, 0, 0));
#else
	GS_WRITE(DISPFB_1,GS_DISPFB(336, m_width / 64, 0, 0, 1));
	GS_WRITE(DISPFB_2,GS_DISPFB(336, m_width / 64, 0, 0, 0));
#endif

#endif

	m_backBufferEnv.setAddress(m_backBuffer, m_width / 64, m_psm);

	masterChain.addSrcEndTag(0, VIF_MSKPATH3(0));
	masterChain.endPacket();

	m_textureSystem.end();

#ifdef ZENIC_PS2
	FlushCache(0); // no no
#endif

	Dma::waitGif();
	Dma::sendChainGif(m_backBufferEnv.chain());
	Dma::waitGif();

#ifdef ZENIC_PS2
	FlushCache(0); // no no
#endif

	static int p = 0;

	p++;


	if (p == 700)
	{
		//screenShot("host:frames/screen.tga");
		//::ps2_screenshot_file("host:frames/screem.tga", m_frontBuffer * 32, 512, 448, 0); 
		//Vu1::dumpOnce();
	}

	//launch masterchain

	m_textureSystem.kick();
	MasterChain::instance()->kick();

	flipDmaBuffer(p);

#ifndef PS2_EMU
	if (m_renderFrames)
	{
		if (m_frameCount < 3350)
		{
			char temp[100];
			sprintf(temp, "host:frames/frame%06d.tga", m_frameCount);
			screenShot(temp); 
		}
	}

	++m_frameCount;

#endif

#ifndef PS2_EMU
	if (m_screenShot)
	{
		char temp[100];
		sprintf(temp, "host:frames/frame%06d.tga", m_frameCount);

		::ps2_screenshot_file(temp, m_backBuffer * 32, 512, 448, 1); 
		++m_frameCount;

		m_screenShot = false;
	}
#endif


	if (m_renderFrames)
		m_currentField = !m_currentField;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::beginScene() { return true; }
bool Renderer::endScene() { return true; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::addClear()
{
	Dma& masterChain = MasterChain::instance()->chain();

	uint offsetX = 2048 - m_width/2;
	uint offsetY = 2048 - m_height/2;

	masterChain.addSrcCntTag(0, VIF_DIRECT(7));
	masterChain.add128(GIFTAG(6, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	masterChain.add128(GS_TEST(1, 7, 0xFF, 0, 0, 0, 1, 1 ),GS_REG(TEST_1));
	masterChain.add128(GS_PRIM(gs::Sprite, gs::Flat, 0, 0, 0, 0, gs::Stq, gs::Context1, 0 ), GS_REG(PRIM));
	masterChain.add128(GS_RGBAQ(m_r, m_g, m_b, 0, 0.0f), GS_REG(RGBAQ));
	masterChain.add128(GS_XYZ2((offsetX)<<4, (offsetY)<<4, 0 ),GS_REG(XYZ2));
	masterChain.add128(GS_XYZ2((offsetX+m_width)<<4, (offsetY+m_height)<<4, 0 ),GS_REG(XYZ2));
	masterChain.add128(GS_TEST(1, 7, 0xFF, 0, 0, 0, 1, 2 ),GS_REG(TEST_1));
	masterChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::flipDmaBuffer(uint field)
{
	MasterChain::instance()->flip();
	m_textureSystem.flip();

	addClear();

/*
	{
		uint offsetX = 2048 - m_width/2;
		uint offsetY = 2048 - m_height/2;

		PostEffectHelper hlp(*this, MasterChain::instance()->chain());
		hlp.setRgba(0, 0, 0, 0);
		hlp.setTest(1, 7, 0xFF, 0, 0, 0, 1, 1);
		hlp.blit(0, 0, 640, 240);
		hlp.setTest(1, 7, 0xFF, 0, 0, 0, 1, 2);

		if (!(field & 1))
		{
			hlp.setXyoffset((2048 - m_width / 2) * 16, (2048 - m_height / 2) * 16);
		}
		else
		{
			hlp.setXyoffset(((2048 - m_width / 2) * 16) + 8, ((2048 - m_height / 2) * 16) + 8);
		}
	}
*/

	m_camera.addToChain();

	if (m_sceneCamera)
	{
		m_camera.setCameraTransform(m_sceneCamera->transform());
		//const Vector3& pos = m_sceneCamera->transform().position();
		//ZENIC_INFO(" pos " << pos.x << " " << pos.y << " " << pos.z);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void WaitDMA()
{
#ifdef ZENIC_PS2
	asm __volatile__("0:");
	asm __volatile__("bc0t 0f" );
	asm __volatile__("nop");
	asm __volatile__(" bc0t 0f" );
	asm __volatile__("nop");
	asm __volatile__(" bc0t 0f" );
	asm __volatile__("nop");
	asm __volatile__("bc0f 0b");
	asm __volatile__("nop");
	asm __volatile__("0:" );
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::beginRenderModels()
{
	Vu1::instance()->clearCache();

	Dma& dmaChain = MasterChain::instance()->chain();

	dmaChain.addSrcCntTag(0);
	dmaChain.add32(VIF_BASE(22));
	dmaChain.add32(VIF_OFFSET(523));
	dmaChain.add32(VIF_STMOD(0));
	dmaChain.add32(VIF_STCYCL(1, 1));
	dmaChain.endPacket();

	// GifTags

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 3, 19));

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Sprite, gs::Gouraud, gs::Texturing, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, 0, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 2));
	dmaChain.add64(0x51);

	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderScene(SceneInfo* sceneInfo, bool /*printInfo*/)
{
	bool glowInit = false;
	Model::resetCollectedModelCount();

	Dma& dmaChain = MasterChain::instance()->chain();

	beginRenderModels();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render opaque non-textured & textured

	renderModelsOpaque(sceneInfo->models(), 0);

	Texture** textures = sceneInfo->textures().buffer();

	Vu1::instance()->clearCache();

	for (uint i = 0; i < sceneInfo->textures().count(); ++i)
		renderModelsOpaque(textures[i]->models(), textures[i]);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render opaque non-textured & textured alphablended

	renderModelsAlpha(sceneInfo->models(), 0);

	Vu1::instance()->clearCache();

	for (uint i = 0; i < sceneInfo->textures().count(); ++i)
		renderModelsAlpha(textures[i]->models(), textures[i]);

	// Restart models again (may be reseted by alpha objects)

	beginRenderModels();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render alpha disolve models

	for (uint i = 0; i < m_alphaDisolveCount; ++i)
	{
		ZENIC_ASSERT(m_alphaDisolveModels[i].fftTexture);
		ZENIC_ASSERT(m_alphaDisolveModels[i].model);

		renderAlphaDissolveEnvmap(m_alphaDisolveModels[i].fftTexture, m_alphaDisolveModels[i].model,
								  m_alphaDisolveModels[i].matrix);
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render glenz objects if found

	Vu1::instance()->clearCache();

	if (GlenzNode::collectedCount() > 0) 
		renderGlenzNodes(GlenzNode::collectedNodes(), GlenzNode::collectedCount());

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render Glowing objects to offscreen buffer and blend back if any objects were found

	uint glowCount = renderModelsGlow(sceneInfo->models(), 0, glowInit);

	Vu1::instance()->clearCache();

	for (uint i = 0; i < sceneInfo->textures().count(); ++i)
		glowCount += renderModelsGlow(textures[i]->models(), textures[i], glowInit);

	if (glowCount > 0)
	{
		PostEffectHelper hlp(*this, dmaChain);
		//hlp.glareOffScreen(336, 0.0f, 2, 0.9f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 2, 1.2f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 2, 1.0f, 1.2f);	
	}

	GlenzNode::resetCollected();
	m_alphaDisolveCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelsOpaque(Array<Model*>& models, Texture* texture)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	Model** modelObjects = models.buffer();
	bool hasUploadedTexture = false;

	for (uint i = 0; i < models.count(); ++i)
	{
		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(modelObjects[i]->modelData());

		// Render addtive materials in pass afterwards.

		if (m_disableOrgGlowObjects)
		{
			if (modelData->material()->glow())
				continue;
		}

		if (modelData->material()->additive())
			continue;

		if (!hasUploadedTexture && texture)
		{
			m_textureSystem.addSync(dmaChain);
			m_textureSystem.setTexture(0, texture);
			m_textureSystem.sync();
			m_textureSystem.activateTexture(dmaChain, texture);
			hasUploadedTexture = true;
		}

		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = models[i]->worldTransform().matrix();

		for (uint j = 0; j < 16 ; ++j)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();

		renderModel(modelObjects[i], 0, false);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Renderer::renderModelsGlow(Array<Model*>& models, Texture* texture, bool& glowInit)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	Model** modelObjects = models.buffer();

	uint glowCount = 0;
	bool hasUploadedTexture = false;

	for (uint i = 0; i < models.count(); ++i)
	{
		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(modelObjects[i]->modelData());

		// Render addtive materials in pass afterwards.

		if (!modelData->material()->glow())
			continue;

		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = models[i]->worldTransform().matrix();

		for (uint j = 0; j < 16 ; ++j)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();

		// We deal with AlphaDissolve seperate

		if (!hasUploadedTexture && texture && modelData->shaderPipeline() != ModelData::AlphaDissolveEnvmapped)
		{
			m_textureSystem.addSync(dmaChain);
			m_textureSystem.setTexture(0, texture);
			m_textureSystem.sync();
			m_textureSystem.activateTexture(dmaChain, texture);
			hasUploadedTexture = true;
		}

		if (!glowInit)
		{
			// Clear the temp buffer

			PostEffectHelper hlp(*this, dmaChain);
			hlp.setFrame(336, 8, 0, 0);
			hlp.setRgba(0, 0, 0, 0);
			hlp.blit(0, 0, 512, 448);
			hlp.setTest(1, 7, 0xFF, 0, 0, 0, 1, 2);
			hlp.setXyoffset((2048 - 512 / 2) * 16, (2048 - 448 / 2) * 16);
			hlp.flushRegisters();
			glowInit = true;
		}

		renderModel(modelObjects[i], 0, false);
		++glowCount;
	}

	return glowCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelsAlpha(Array<Model*>& models, Texture* texture)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	Model** modelObjects = models.buffer();

	bool hasUploadedTexture = false;

	// Set alpha blending on the giftags and set proper blendmode

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 3, 19));

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Sprite, gs::Gouraud, gs::Texturing, 0, 1, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, 1, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, 0, 0, 1, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 2));
	dmaChain.add64(0x51);

	dmaChain.endPacket();


	//

	dmaChain.addSrcCntTag(0, VIF_DIRECT(3));
	dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_ALPHA(0, 2, 2, 1, 0x80), GS_REG(ALPHA_1));
	dmaChain.add128(GS_ZBUF(224, gs::Z24, 1), GS_REG(ZBUF_1));	// Dont update the zbuffer during alphablend
	dmaChain.endPacket();

	//

	for (uint i = 0; i < models.count(); ++i)
	{
		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(modelObjects[i]->modelData());

		// Render addtive materials in pass afterwards.

		if (!modelData->material()->additive())
			continue;

		if (!hasUploadedTexture && texture)
		{
			m_textureSystem.addSync(dmaChain);
			m_textureSystem.setTexture(0, texture);
			m_textureSystem.sync();
			m_textureSystem.activateTexture(dmaChain, texture);
			hasUploadedTexture = true;
		}

		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = models[i]->worldTransform().matrix();

		for (uint j = 0; j < 16 ; ++j)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();

		renderModel(modelObjects[i], 0, false);
	}

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_ZBUF(224, gs::Z24, 0), GS_REG(ZBUF_1));	// zbuffer update on
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderAdditveModel(Model* model, const Matrix4& matrixRotation)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	// Set alpha blending on the giftags and set proper blendmode

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 3, 19));

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Sprite, gs::Gouraud, gs::Texturing, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, 0, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 2));
	dmaChain.add64(0x51);

	dmaChain.endPacket();

	//

	dmaChain.addSrcCntTag(0, VIF_DIRECT(3));
	dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_ALPHA(0, 2, 2, 1, 0x80), GS_REG(ALPHA_1));
	dmaChain.add128(GS_ZBUF(224, gs::Z24, 1), GS_REG(ZBUF_1));	// Dont update the zbuffer during alphablend
	dmaChain.endPacket();

	//

	ps2::ModelData* modelData = static_cast<ps2::ModelData*>(model->modelData());

	// Render addtive materials in pass afterwards.
/*
	m_textureSystem.addSync(dmaChain);
	m_textureSystem.setTexture(0, &material->baseTexture());
	m_textureSystem.sync();
	m_textureSystem.activateTexture(dmaChain, &material->baseTexture());
*/
	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

	const float* matrix = (model->worldTransform() * matrixRotation).matrix();

	for (uint j = 0; j < 16 ; ++j)
		dmaChain.addFloat(*matrix++);

	dmaChain.endPacket();

	renderModel(model, 0, true);

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_ZBUF(224, gs::Z24, 0), GS_REG(ZBUF_1));	// zbuffer update on
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModels(Model** models, uint modelsCount)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	beginRenderModels();

	// Process all models

	for (uint i = 0; i < modelsCount; ++i)
	{
		// Clean this up

		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(models[i]->modelData());

		// Render addtive materials in pass afterwards.

		if (modelData->material()->additive())
			continue;

		//

		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = models[i]->worldTransform().matrix();

		for (uint j = 0; j < 16 ; ++j)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();

		renderModel(models[i], 0, true);
	}

	uint glowCount = 0;
	bool hasSetGlow = false;

	for (uint i = 0; i < modelsCount; ++i)
	{
		// Clean this up

		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(models[i]->modelData());

		// Render addtive materials in pass afterwards.

		if (!modelData->material()->glow())
			continue;

		if (!hasSetGlow)
		{
			PostEffectHelper hlp(*this, dmaChain);
			hlp.setFrame(336, 8, 0, 0);
			hlp.setRgba(0, 0, 0, 0);
			hlp.blit(0, 0, 512, 448);
			hlp.setTest(1, 7, 0xFF, 0, 0, 0, 1, 2);
			hlp.setXyoffset((2048 - 512 / 2) * 16, (2048 - 448 / 2) * 16);
			hlp.flushRegisters();
			hasSetGlow = true;
		}
		
		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = models[i]->worldTransform().matrix();

		for (uint j = 0; j < 16 ; ++j)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();

		renderModel(models[i], 0, true);

		++glowCount;
	}


	//if (glowCount > 0)
	//{
		//PostEffectHelper hlp(*this, dmaChain);
		//hlp.glareOffScreen(336, 0.0f, 2, 0.9f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 2, 1.2f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 2, 1.0f, 1.2f);
		//PostEffectHelper hlp(*this, dmaChain);
		//hlp.glareOffScreen(336, 0.0f, 2, 0.9f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 2, 1.2f, 1.2f);
		//hlp.glareOffScreen(336, 0.0f, 1, 0.5f, 0.6f);
	//}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModel(const Model* model, const Matrix4* addMatrix, bool setMaterial)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	ps2::ModelData* modelData = static_cast<ps2::ModelData*>(model->modelData());

	if (addMatrix)
	{
		dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 4, 10));

		const float* matrix = addMatrix->matrix();

		for (uint i = 0; i < 16 ; ++i)
			dmaChain.addFloat(*matrix++);

		dmaChain.endPacket();
	}

	switch (modelData->shaderPipeline())
	{
		case ModelData::IntTexturedNonShaded : renderModelIntTextured(modelData, dmaChain); break;
		case ModelData::FloatTexturedNonShaded : renderModelFloatTextured(modelData, dmaChain); break;
		case ModelData::IntFlatNonShaded : renderModelIntFlat(modelData, dmaChain); break;
		case ModelData::FloatFlatNonShaded : renderModelFloatFlat(modelData, dmaChain); break;
		case ModelData::Sprite :
		{
			ZENIC_INFO("Sprites not supported for now");
			break;
		}
		case ModelData::Dynamic :
		{
			ZENIC_INFO("Dynamic ShaderPipeline not supported for now");
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelIntTextured(const ModelData* modelData, Dma& dmaChain, bool setMaterial)
{
#ifndef PS2_EMU
	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::IntTexturedNonShaded, 
											&IntTexturedNonShaded_CodeStart, 
											&IntTexturedNonShaded_CodeEnd);
#else
	s32 start = 0;
#endif

	renderModelTextured(modelData, start, dmaChain, setMaterial);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelFloatTextured(const ModelData* modelData, Dma& dmaChain, bool setMaterial)
{
#ifndef PS2_EMU
	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::FloatTexturedNonShaded, 
											&FloatTexturedNonShaded_CodeStart, 
											&FloatTexturedNonShaded_CodeEnd);
#else
	s32 start = 0;
#endif

	renderModelTextured(modelData, start, dmaChain, setMaterial);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelIntFlat(const ModelData* modelData, Dma& dmaChain, bool setMaterial)
{
#ifndef PS2_EMU
	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::IntFlatNonShaded, 
											&IntFlatNonShaded_CodeStart, 
											&IntFlatNonShaded_CodeEnd);
#else
	s32 start = 0;
#endif

	renderModelFlat(modelData, start, dmaChain, setMaterial);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelFloatFlat(const ModelData* modelData, Dma& dmaChain, bool setMaterial)
{
#ifndef PS2_EMU
	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::IntFlatNonShaded, 
											&IntFlatNonShaded_CodeStart, 
											&IntFlatNonShaded_CodeEnd);
#else
	s32 start = 0;
#endif

	renderModelFlat(modelData, start, dmaChain, setMaterial);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelTextured(const ModelData* modelData, s32 vu1StartAddress, Dma& dmaChain, bool setMaterial)
{
	if (modelData->material() && setMaterial)
	{
		const Material* material = modelData->material();

		m_textureSystem.addSync(dmaChain);
		m_textureSystem.setTexture(0, &material->baseTexture());
		m_textureSystem.sync();
		m_textureSystem.activateTexture(dmaChain, &material->baseTexture());
	}

	dmaChain.addSrcCntTag(0);

	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_MSCAL(vu1StartAddress));

	dmaChain.endPacket();

	dmaChain.addSrcCallTag(modelData->primaryChain());
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelFlat(const ModelData* modelData, s32 vu1StartAddress, Dma& dmaChain, bool setMaterial)
{
	dmaChain.addSrcCntTag(0);

	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_MSCAL(vu1StartAddress));

	dmaChain.endPacket();

	dmaChain.addSrcCallTag(modelData->primaryChain());
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderGlenzNodes(const GlenzNode** nodes, uint nodeCount)
{
/*
	Dma& dmaChain = MasterChain::instance()->chain();

	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::AlphaDissolveEnvmapped, 
											&AlphaDissolveEnvmapped_CodeStart, 
											&AlphaDissolveEnvmapped_CodeEnd);


	m_textureSystem.addSync(dmaChain);
	m_textureSystem.setTexture(0, &material->baseTexture());
	m_textureSystem.sync();
	m_textureSystem.activateTexture(dmaChain, &material->baseTexture());

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render with envmapping.
	// Enable alpha blending

	dmaChain.addSrcCntTag(0, VIF_UNPACK(zenic::ps2::vif::V4_32, 1, 20));

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, 1, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.endPacket();

	// Set alpha blend func

	// Render the envmapped object with addtive alphablending that uses the alpha already written in the framebuffer
	// Render to all channels

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_ALPHA(0, 2, 1, 1, 0), GS_REG(ALPHA_1));
	dmaChain.endPacket();

	// Start the drawing

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_16, 1, 18));
	dmaChain.add16(1);
	dmaChain.add16(1);
	dmaChain.add16(1);
	dmaChain.add16(1);

	dmaChain.align(4);

	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_MSCAL(start));

	dmaChain.align(16);
	dmaChain.endPacket();

	dmaChain.addSrcCallTag(modelData->primaryChain());
	dmaChain.endPacket();
*/

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderAlphaDissolveEnvmap(Texture* texture, const Model* model, const Matrix4& matrix)
{
	Dma& dmaChain = MasterChain::instance()->chain();

	ZENIC_ASSERT(model);

	beginRenderModels();

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 8, 10));

	Matrix4 matrixRotScale = matrix * model->transform();

	// Rotate and scale for vertices

	const float* matrixTemp = matrixRotScale.matrix();

	for (uint j = 0; j < 16 ; ++j)
		dmaChain.addFloat(*matrixTemp++);

	matrixTemp = matrix.matrix();

	// Only rotation for the normals, no scale

	for (uint j = 0; j < 16 ; ++j)
		dmaChain.addFloat(*matrixTemp++);

	dmaChain.endPacket();

	renderModelAlphaDissolveEnvmap(texture, (ps2::ModelData*)model->modelData(), dmaChain);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModelAlphaDissolveEnvmap(Texture* texture, const ModelData* modelData, Dma& dmaChain)
{

	ZENIC_ASSERT(modelData);

	{
		// clear alpha channel
		PostEffectHelper hlp(*this, dmaChain);
		hlp.setFrame(currentScreen(), 8, 0, 0x00ffffff);
		hlp.setRgba(0, 0, 0, 0);
		//hlp.blit(0, 0, 512, 448);
		hlp.setTest(0, 7, 0xFF, 0, 0, 0, 1, 2);
		hlp.setXyoffset((2048 - 512 / 2) * 16, (2048 - 448 / 2) * 16);
		hlp.flushRegisters();
	}

#ifndef PS2_EMU
	s32 start = Vu1::instance()->getProgram(dmaChain, Vu1::AlphaDissolveEnvmapped, 
											&AlphaDissolveEnvmapped_CodeStart, 
											&AlphaDissolveEnvmapped_CodeEnd);
#endif


	m_textureSystem.addSync(dmaChain);
	m_textureSystem.setTexture(0, texture);
	m_textureSystem.sync();
	m_textureSystem.activateTexture(dmaChain, texture, true);

	dmaChain.addSrcCntTag(0, VIF_DIRECT(3));
	dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_TEX1(0, 0, 0, 0, 0, 0, 0), GS_REG(TEX1_1)); // Bilinear filtering off
	dmaChain.add128(GS_ZBUF(224, gs::Z24, 0), GS_REG(ZBUF_1));	// Dont update the zbuffer during alphablend
	dmaChain.endPacket();

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_16, 1, 18));
	dmaChain.add16(0);
	dmaChain.add16(0);
	dmaChain.add16(0);
	dmaChain.add16(0);

	dmaChain.align(4);

	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
#ifndef PS2_EMU
	dmaChain.add32(VIF_MSCAL(start));
#endif

	dmaChain.align(16);
	dmaChain.endPacket();

	dmaChain.addSrcCallTag(modelData->primaryChain());
	dmaChain.endPacket();

	// Render the envmapped object with addtive alphablending that uses the alpha already written in the framebuffer
	// Render to all channels

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_FRAME(currentScreen(), 8, 0, 0), GS_REG(FRAME_1));
	dmaChain.endPacket();

	// Real envmap

	const Material* material = modelData->material();

	ZENIC_ASSERT(material);
	ZENIC_ASSERT(&material->baseTexture());

	m_textureSystem.addSync(dmaChain);
	m_textureSystem.setTexture(0, &material->baseTexture());
	m_textureSystem.sync();
	m_textureSystem.activateTexture(dmaChain, &material->baseTexture());

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Render with envmapping.
	// Enable alpha blending

	dmaChain.addSrcCntTag(0, VIF_UNPACK(zenic::ps2::vif::V4_32, 1, 20));

	dmaChain.add64(GIFTAG(0, 0, 1, GS_PRIM(gs::Tristrip, gs::Gouraud, gs::Texturing, 0, 0, 0, gs::Stq, 
										   gs::Context1, 0), gif::Packed, 3));
	dmaChain.add64(0x512);

	dmaChain.endPacket();

	// Set alpha blend func
	
	// Render the envmapped object with addtive alphablending that uses the alpha already written in the framebuffer
	// Render to all channels

	dmaChain.addSrcCntTag(0, VIF_DIRECT(3));
	dmaChain.add128(GIFTAG(2, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_TEX1(0, 0, 1, 1, 0, 0, 0), GS_REG(TEX1_1)); // Bilinear filtering on
	//dmaChain.add128(GS_ALPHA(1, 0, 1, 0, 0), GS_REG(ALPHA_1));
	dmaChain.add128(GS_TEST(0, 0, 0, 0, 1, 1, 1, 2), GS_REG(TEST_1));
	dmaChain.endPacket();

	// Start the drawing

	const Vector3& cameraPos = m_camera.position();

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_32, 1, 19));
	dmaChain.addFloat(cameraPos.x);
	dmaChain.addFloat(cameraPos.y);
	dmaChain.addFloat(cameraPos.z);
	dmaChain.addFloat(1.0f);
	dmaChain.endPacket();

	dmaChain.addSrcCntTag(VIF_STCYCL(1, 1), VIF_UNPACK(zenic::ps2::vif::V4_16, 1, 18));
	dmaChain.add16(0);
	dmaChain.add16(0);
	dmaChain.add16(0);
	dmaChain.add16(0);

	dmaChain.align(4);

	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
	dmaChain.add32(VIF_NOP());
#ifndef PS2_EMU
	dmaChain.add32(VIF_MSCAL(start));
#endif

	dmaChain.align(16);
	dmaChain.endPacket();

	dmaChain.addSrcCallTag(modelData->primaryChain());
	dmaChain.endPacket();

	dmaChain.addSrcCntTag(0, VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1),gif::A_D);
	dmaChain.add128(GS_TEST(0, 0, 0, 0, 0, 0, 1, 2), GS_REG(TEST_1));
	//dmaChain.add128(GS_ZBUF(224, gs::Z24, 0), GS_REG(ZBUF_1));	// update the zbuffer
	dmaChain.endPacket();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::addAlphaDissolveModel(Texture* texture, const Model* model, const Matrix4& matrix)
{
	ZENIC_ASSERT(m_alphaDisolveCount < 20);
	m_alphaDisolveModels[m_alphaDisolveCount].fftTexture = texture;
	m_alphaDisolveModels[m_alphaDisolveCount].model = model;
	m_alphaDisolveModels[m_alphaDisolveCount].matrix = matrix;
	++m_alphaDisolveCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderScreenBlur(const Texture* alphaMask)
{
	m_textureSystem.setTexture(0, alphaMask);
	m_textureSystem.sync();
	m_textureSystem.activateTexture(MasterChain::instance()->chain(), alphaMask);

	PostEffectHelper hlp(*this, MasterChain::instance()->chain());

	hlp.setFrame(224, 8);
	hlp.blit(0, 0, 512, 448, false, true);

	hlp.blur(224, 336, 8, 512, 448, 4, 1.0f, 0);

	hlp.setFrame(currentScreen(), 8, 0, 0x00ffffff);
	hlp.setTex0Pal((450*32) + 32, (450*32), 4, 19, 8, 8, 1, 1);
	hlp.blit(0, 0, 512, 448, 0.0f, 0.0f, 256.0f, 256.0f, false, true);

	hlp.setClamp(2, 2, 0, 511, 0, 447);

	hlp.setFrame();
	hlp.setTex0(224, 8);
	hlp.setAlpha(1, 0, 1, 0, 0);
	hlp.blit(0, 0, 512, 448, -1.0, -1.0f, 511.0f, 447.0f, true, true);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u64 Renderer::makeDisplayReg()
{
	uint mode = 1;
	//uint frame = 1;
	uint dx = 0;
	uint dy = 0;
	uint maxHeight = 0;

	//ntsc
	if (mode == 1)
	{
		dx = 0x27c;
		dy = 50;
		maxHeight = 448;
	}
	else
	{
		dx = 0x290;
		dy = 72;
		maxHeight = 512;
	}

	uint offset = (maxHeight - m_height) >> 1;

	dy += offset;

	dx = 0;
	dy = 0; 

	uint h = m_height;
	uint w = m_width;

	return (((u64)(((2560+w-1)/w)-1) << 23) |
					((u64)((h<<(0))-1) << 44) |
        					(((u64)636+(dx*((2560+w-1)/w)))&0xfff)|
					((u64)((50+dy) & 0xfff)<<12) |
					((u64)((((2560+w-1)/w)*w)-1) << 32));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::screenShot(const char* filename)
{
#ifndef PS2_EMU
	u32 buffer[512*4];  // max 1024*32bit for a line, should be ok
	u32 bufferOut[512*3];

	unsigned char header[18] = 
	{
	   0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	   0, 0, // Width 
	   0, 0, // Heigth
	  24, 0
	};

	s16* p_header = (s16*)&header;

	/////////////////////////////////////////////////////////////////////////////
	// setup tga header

	p_header[6] = (s16)512;
	p_header[7] = (s16)448;

	int fileHandle = fioOpen(filename, O_CREAT|O_WRONLY);

	// make sure we could open the file for output

	if (fileHandle < 0)
	  return;
/*
	u8* screenBuffer = zenic_new u8[18 + (512 * 448 * 3)];
	u8* screenBufferTemp = screenBuffer;

	for (uint i = 0; i < 18; ++i)
		*screenBufferTemp++ = header[i];
*/
	fioWrite(fileHandle, (const void*)&header, 18);

	/////////////////////////////////////////////////////////////////////////////
	// Check if we have a tempbuffer, if we do we use it 

	for (uint y = 0; y < 448; y++)
	{
	  ps2_screenshot(buffer, m_frontBuffer * 32, 0, (448-1)-y, 512, 1, 0);

	  u8* bufferOutTemp = (u8*)&bufferOut;
	  u8* bufferInTemp = (u8*)&buffer;

	  for (uint x = 0; x < 512; ++x)
	  {
		  //*screenBufferTemp++ = bufferInTemp[2];
		  //*screenBufferTemp++ = bufferInTemp[1];
		  //*screenBufferTemp++ = bufferInTemp[0];
		  *bufferOutTemp++ = bufferInTemp[2];
		  *bufferOutTemp++ = bufferInTemp[1];
		  *bufferOutTemp++ = bufferInTemp[0];
		  bufferInTemp += 4;
	  }

	  fioWrite(fileHandle, bufferOut, 512*3);
	}

	//fioWrite(fileHandle, screenBuffer, 18 + (512 * 448 * 3)); 

	//zenic_delete [] screenBuffer;
	fioClose(fileHandle);

	return;
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This really needs to be rewritten

void Renderer::initScene(SceneInfo* sceneInfo)
{
	ZENIC_ASSERT(sceneInfo);
	ZENIC_ASSERT(sceneInfo->rootNode());

	Model::resetCollectedModelCount();
	sceneInfo->rootNode()->collect(sceneInfo->cameras(), Node::CameraType);
	sceneInfo->rootNode()->update(0.0f);
	
	Model** models = Model::collectedModels();
	uint modelsCount = Model::collectedModelsCount();

	for (uint i = 0; i < modelsCount; ++i)
	{
		ps2::ModelData* modelData = static_cast<ps2::ModelData*>(models[i]->modelData());

		switch (modelData->shaderPipeline())
		{
			case ModelData::IntTexturedNonShaded : 
			case ModelData::FloatTexturedNonShaded : 
			case ModelData::AlphaDissolveEnvmapped : 
			{
				const Material* material = modelData->material();
				Texture* texture = (Texture*)&material->baseTexture();

				bool foundTexture = false;

				for (uint k = 0; k < sceneInfo->textures().count(); ++k)
				{
					if (texture == sceneInfo->textures().buffer()[k])
					{
						foundTexture = true;
						break;
					}
				}

				if (!foundTexture)
					sceneInfo->textures().pushBack(texture);

				texture->models().pushBack(models[i]);

				break;
			}

			case ModelData::FloatFlatNonShaded :
			case ModelData::IntFlatNonShaded :
			{
				sceneInfo->models().pushBack(models[i]);
				break;
			}

			case ModelData::Sprite :
			{
				ZENIC_INFO("Sprites not supported for now");
				break;
			}

			case ModelData::Dynamic :
			{
				ZENIC_WARNING("Dynamic objects not supported for now.");
				break;
			}
		}
	}

#ifndef ZENIC_FINAL

	ZENIC_INFO("Found " << sceneInfo->textures().count() << " textures in scene");

	for (uint i = 0; i < sceneInfo->textures().count(); ++i)
		ZENIC_INFO("Texture " << i << " got " << sceneInfo->textures().buffer()[i]->models().count() << " objects assigned to it"); 

	ZENIC_INFO("Found " << sceneInfo->models().count() << " objects in scene with no texture");

#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
u64 Renderer::makeDisplayReg()
{
	int s = 1;

	uint h = m_height;
	uint w = m_width;

	uint dx = 0;
	uint dy = 0;

	return 0
	| ((u64) (636+(dx*((2560+w-1)/w)))&0xfff)
	| ((u64) ((50+dy) & 0xfff)             << 12)
	| ((u64) (((2560+w-1)/w)-1)            << 23)
	| ((u64) ((((2560+w-1)/w)*w)-1)        << 32)
	| ((u64) ((h<<(1))-s) << 44);
}
*/

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	}
}

