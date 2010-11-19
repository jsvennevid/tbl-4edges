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

