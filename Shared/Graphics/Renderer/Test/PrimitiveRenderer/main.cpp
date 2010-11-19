///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Graphics/Renderer/Renderer.h>
#include <Shared/Graphics/Renderer/PrimitiveRenderer.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Base/Math/Vector3.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	zenic::ps2::Renderer::init();
	zenic::PrimitiveRenderer::create();

	int i = 0;

	while(1)
	{
		zenic::PrimitiveRenderer::instance().triangle(zenic::Vector3(1.0f, 1.0f, 12.0f), zenic::Vector3(-1.0f, 1.0f, 12.0f),
												     zenic::Vector3(0.0f, -1.0f, 12.0f));

		zenic::ps2::Renderer::flip();


		i++;

		if (i == 2)
			zenic::ps2::hardware::Vu1::dumpOnce("host:vu1micromem","host:vu1mem");
	}

	return 0;
}
