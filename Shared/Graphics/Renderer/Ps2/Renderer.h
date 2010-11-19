///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef zenic_ps2_Renderer_h
#define zenic_ps2_Renderer_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Hardware/Ps2/Gs.h>
#include <Shared/Hardware/Ps2/Dma.h>
#include <Shared/Graphics/Renderer/Ps2/DrawingEnviroment.h>
#include <Shared/Graphics/Renderer/Ps2/TextureSystem.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Renderer/Renderer.h>
#include <Shared/Base/Storage/Singleton.h>
#include <Shared/Base/Storage/Array.h>
#include "Camera.h"
#include "RenderSettings.h"

#ifdef PS2_EMU
#include <Shared/External/ps2emu/Display.h>
#endif

#ifndef PS2_EMU
#include <debug.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Model;
	class Camera;
	class SceneInfo;
	class GlenzNode;
	
	namespace ps2
	{
		class ShRenderer;
		class CubeRenderer;
		class Dma;
		class ModelData;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Renderer : public zenic::Renderer
{
public:

	Renderer(bool renderFrames = false);
	~Renderer();




	void init();

	bool create();
	bool destroy();

	bool beginScene();
	bool endScene();

	bool isRunning() const;

	void flip();

	Matrix4* viewScreenMatrix();

	u32 width();
	u32 height();

	u32 currentScreen();
	u32 backBuffer();
	void setClearColor(uint r, uint g, uint b);

	void initScene(SceneInfo* sceneInfo);
	void renderScene(SceneInfo* sceneInfo, bool printInfo = false);
	void renderModels(Model** models, uint modelsCount);

	Camera& camera();
	const RenderSettings& renderSettings() const;
	Dma& gifChain();

	zenic::Camera* sceneCamera() const;
	void setSceneCamera(const zenic::Camera* sceneCamera);

	TextureSystem& textureSystem();
	CubeRenderer& cubeRenderer();
	ShRenderer& shRenderer();

	void setRenderFrames(bool renderFrames);

	void screenShot(const char* filename);
	void screenShot();
	void beginRenderModels();
	void renderModel(const Model* model, const Matrix4* addMatrix, bool setMaterial = true);
	void renderScreenBlur(const Texture* alphaMask);

	void addAlphaDissolveModel(Texture* texture, const Model* model, const Matrix4& matrix);

	void renderAlphaDissolveEnvmap(Texture* texture, const Model* model, const Matrix4& matrix);
	void renderModelAlphaDissolveEnvmap(Texture* texture, const ModelData* modelData, Dma& dmaChain);

	void setBlurSetting(uint setting);

	bool disableOrgGlowObjects() const;
	void setDisableOrgGlowObjects(bool disableOrgGlowObjects);

	void renderAdditveModel(Model* model, const Matrix4& matrixRotation);

private:
	u64 makeDisplayReg();

	void flipDmaBuffer(uint field);
	void buildClearTag();
	void addClear();

	bool evenOddField();

	// TODO: Fix const correct

	void renderGlenzNodes(const GlenzNode** nodes, uint nodeCount);
	void renderModelsOpaque(Array<Model*>& models, Texture* texture);
	uint renderModelsGlow(Array<Model*>& models, Texture* texture, bool& initGlow);
	void renderModelsAlpha(Array<Model*>& models, Texture* texture);

	void renderModel(const Model* model, float scale, float angle, Dma& dmaChain, bool setMaterial = true);
	void renderModelFlat(const ModelData* modelData, s32 vu1StartAddress, Dma& dmaChain, bool setMaterial = true);
	void renderModelTextured(const ModelData* modelData, s32 vu1StartAddress, Dma& dmaChain, bool setMaterial = true);

	void renderModelIntTextured(const ModelData* modelData, Dma& dmaChain, bool setMaterial = true);
	void renderModelFloatTextured(const ModelData* modelData, Dma& dmaChain, bool setMaterial = true);
	void renderModelIntFlat(const ModelData* modelData, Dma& dmaChain, bool setMaterial = true);
	void renderModelFloatFlat(const ModelData* modelData, Dma& dmaChain, bool setMaterial = true);

	//static Dma m_masterChain;

	struct AlphaDisolveModel
	{
		Texture* fftTexture;
		const Model* model;
		Matrix4 matrix;
	};

	RenderSettings m_settings;
	DrawingEnviroment m_backBufferEnv;
	TextureSystem m_textureSystem;

	Texture m_spectrumTexture;

	AlphaDisolveModel m_alphaDisolveModels[20];

	ps2::Camera m_camera;
	Matrix4 m_viewScreen;

	CubeRenderer* m_cubeRenderer;
	ShRenderer* m_shRenderer;
	zenic::Camera* m_sceneCamera;

	u32 m_frontBuffer;
	u32 m_backBuffer;
	u32 m_zbuffer;
	u32 m_targetBuffer;

	u32 m_width;
	u32 m_height;
	gs::Psm m_psm;

	uint m_r;
	uint m_g;
	uint m_b;

	u32 m_frameCount;

	bool m_disableOrgGlowObjects;
	bool m_flip;
	bool m_renderFrames;
	bool m_currentField;
	bool m_screenShot;

#ifdef PS2_EMU
	ps2emu::Display m_emuDisplay;
#endif

	uint m_alphaDisolveCount;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const RenderSettings& Renderer::renderSettings() const
{
	return m_settings;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4* Renderer::viewScreenMatrix()
{
	return (Matrix4*)&m_viewScreen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Renderer::width()
{
	return m_width;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Renderer::height()
{
	return m_height;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Renderer::currentScreen()
{
	return m_frontBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Renderer::backBuffer()
{
	return m_backBuffer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setClearColor(uint r, uint g, uint b)
{
	m_r = r;
	m_g = g;
	m_b = b;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Camera& Renderer::camera()
{
	return m_camera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline zenic::Camera* Renderer::sceneCamera() const
{
	return m_sceneCamera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setSceneCamera(const zenic::Camera* sceneCamera)
{
	m_sceneCamera = (zenic::Camera*)sceneCamera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline TextureSystem& Renderer::textureSystem()
{
	return m_textureSystem;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline CubeRenderer& Renderer::cubeRenderer()
{
	return *m_cubeRenderer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ShRenderer& Renderer::shRenderer()
{
	return *m_shRenderer;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::screenShot()
{
	m_screenShot = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setRenderFrames(bool renderFrames)
{
	m_renderFrames = renderFrames;
	m_frameCount = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Renderer::isRunning() const
{
#ifdef PS2_EMU
	return m_emuDisplay.isRunning();
#else
	return true;	
#endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Renderer::disableOrgGlowObjects() const
{
	return m_disableOrgGlowObjects;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setDisableOrgGlowObjects(bool disableOrgGlowObjects)
{
	m_disableOrgGlowObjects = disableOrgGlowObjects;
}
 

	}
}


#endif //zenic_ps2_Renderer_h


