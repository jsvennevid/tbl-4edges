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
