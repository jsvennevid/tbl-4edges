///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Io/File.h>
#include <Shared/Base/Io/FileStream.h>
#include <Shared/Base/Serialize/InBinarySerializer.h>
#include <Shared/Geometry/Ps2/ModelData.h>
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Graphics/Scene/Scene.h>
#include <Shared/Graphics/Scene/SceneInfo.h>
#include <Shared/Graphics/Scene/AnimationController.h>
#include <Shared/Graphics/Renderer/Ps2/Material.h>
#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Graphics/Renderer/Ps2/DebugRenderer.h>
#include <Shared/Hardware/Ps2/MasterChain.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/glu.h>
#include <gl/gl.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		void demoMain();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{   
	zenic::ps2::demoMain();
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static zenic::ps2::Texture _dummy;
static zenic::ps2::Material _dummy2;
static zenic::ps2::ModelData _dummy3;
static zenic::AnimationController _dummy4;

void demoMain()
{
	Renderer renderer;
	renderer.create();

	//renderer.init(0, 0, 1280, 720);
	//renderer.setRenderingMode(opengl::Renderer::BoundingBox);

	DebugRenderer* debugRenderer = zenic_new DebugRenderer(MasterChain::instance()->chain(), renderer.camera());
	DebugRenderer::setInstance(debugRenderer);

	FileStream inStream;
	if (!inStream.open("test2.bin", FileStream::Read))
		return;

	InBinarySerializer inBinary;

	inBinary.load(inStream);
	SceneInfo* scene = static_cast<SceneInfo*>(inBinary[0]);

	float time = 0.0f;
	time = 1.0f / 220.0f;

	//renderer.setCamera(scene->primaryCamera());

	while (1) 
	{
		debugRenderer->begin();
		debugRenderer->triangle(Vector3(-1.0f, -1.0f, 1.0f), 
			                    Vector3( 1.0f, -1.0f, 1.0f), 
								Vector3( 0.0f,  1.0f, 1.0f),
								Color32(23, 23, 23, 0));
		debugRenderer->end();


		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glPushMatrix();
		//glTranslatef(0.0f, 0.0f, -15.0f);

		//Model::resetCollectedModelCount();
		//scene->rootNode()->update(time);

		//renderer.renderModels(Model::collectedModels(), Model::collectedModelsCount());

		//glPopMatrix();

		renderer.flip();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

