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

#ifndef zenic_ps2_ShaderBuilder_h
#define zenic_ps2_ShaderBuilder_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLXSIMaterial;
class CSLXSIShader;

namespace zenic
{
	class String;

	namespace ps2
	{
		class ExporterBackend;
		class Material;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ShaderBuilder
{
public:

	static bool build(ps2::Material* material, ps2::ExporterBackend& backend, CSLXSIMaterial* xsiMaterial);

private:

	enum
	{
		SoftImageTxt2d = 0xbb42a7e2, // Softimage.txt2d-image-explicit.1
		SoftImageConstant = 0x03f4cc45, // Softimage.material-constant.1
		SoftImagePtBillboard = 0x26b8b5f7 // Softimage.pt_billboard.1
	};

	static void textureShader(Material* material, ExporterBackend& backend, CSLXSIShader* xsiShader);
	static void constantShader(Material* material, ExporterBackend& backend, CSLXSIShader* xsiShader);
	static void billboardShader(Material* material, ExporterBackend& backend, CSLXSIShader* xsiShader);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif



