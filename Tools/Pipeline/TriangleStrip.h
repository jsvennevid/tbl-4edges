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

#ifndef zenic_TriangleStrip_h
#define zenic_TriangleStrip_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER)
#pragma warning(disable: 4530)
#endif

#include <vector>

#if defined(_MSC_VER)
#pragma warning(default: 4530)
#endif

#include <Shared/Base/Types.h>
#include <Shared/Base/Memory/DataPtr.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLXSIMesh;

namespace zenic
{
	class DegenerateMesh;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TriangleStrip
{
public:
	
	enum Technique
	{
		Actc,
		Tunnel
	};

	TriangleStrip(Technique technique);
	~TriangleStrip();

	struct StripInfo
	{
		u32* remapTable;
		std::vector<DataPtr<u32>*> stripList;	
	};

	StripInfo process(CSLXSIMesh* mesh); 
	StripInfo process(DegenerateMesh* mesh); 

private:

	StripInfo processActc(const int* indexList, const int indexListCount);

	Technique m_technique;
	u32* m_indices;
	u32 m_indicesCount;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
