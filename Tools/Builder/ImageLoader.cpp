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

#include "ImageLoader.h"
#include "../Pipeline/Ps2/TextureBuilder.h"
#include <Shared/Geometry/Ps2/Texture.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Serialize/OutBinarySerializer.h>
#include <il/il.h>
#include <il/ilu.h>
#include <string>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ImageLoader::load(const char* filename)
{
	ilInit();
	ilBindImage(iluGenImage());

	if (!ilLoadImage((ILstring)filename))
	{
		uint error = ilGetError();
		
		logOut << "Unable to load file " << filename << endl;
		return false;
	}
	
	std::string tempFilename = filename;

	if (tempFilename.find(".tga"))
	{
		//iluFlipImage();
	}

	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	
	m_data = ilGetData();
	
	m_width = ilGetInteger(IL_IMAGE_WIDTH);
	m_height = ilGetInteger(IL_IMAGE_HEIGHT);

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void ImageLoader::process(OutSerializer& serializer, ps2::gs::Psm dest)
{
	// Only for Ps2 textures

	ps2::TextureBuilder textureBuilder;

	ps2::Texture* texture = textureBuilder.createTexture(m_width, m_height, ps2::gs::CT32, dest, 1, m_data, 450);

	if (texture)
		serializer.add(texture);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

