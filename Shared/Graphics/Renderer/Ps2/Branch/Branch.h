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

#ifndef zenic_Branch_h
#define zenic_Branch_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <Shared/Base/Math/Vector3.h>
#include <Shared/Geometry/Ps2/BranchModel.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{
		class BranchSettings;
		class Dma;
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Branch
{
public :

	Branch();
	Branch(const Branch* clone);

	void split();
	void divide(float angleMul = 1.0f, bool hasReachedTarget = false);
	void render(Dma& dmaChain, float deltaTime);
	void createRoot(const Vector3& orgin, const Vector3& direction = Vector3(0.0f, 1.0f, 0.0f));
	void createRoot();
	void update(float deltaTime, bool recursive);
	void clone(const Branch* clone);

	static void init();

	enum
	{
		MaxSides = 6
	};

	bool makeNewDirection(float angleMul = 1.0f, bool hasReatchedTarget = false);
	static s32 beginRender(Dma& dmaChain);

	//BranchModel* model() const;
	void setControlPoints(BranchModel::Vertex* curve, BranchModel::Vertex* curve2, 
						  u16 count, float offset, const Vector3& scale);

//private:

	void generateSection(Vector3* coords, const Vector3& pos, const Vector3& pos2);
	static float floatRandom();
	static uint random();
	Vector3 curvePos();

	Vector3 m_position;
	Vector3 m_direction;
	Vector3 m_curveScale;

	Branch* m_split1;
	Branch* m_split2;

	Vector3* m_strips;
	Vector3* m_topSection;

	BranchModel::Vertex* m_curve;
	BranchModel::Vertex* m_curve2;
	
	void* m_callChain;
	u32	m_section;
	float m_radius;
	float m_offset;

	u16 m_curveCount;
	u16 m_curveTarget;

	static uint m_seed;

	static float s_generateTime;
	static float s_splitTime;
	static BranchSettings* s_settings;
	static float s_cosTable[MaxSides+1];
	static float s_sinTable[MaxSides+1];
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Branch::setControlPoints(BranchModel::Vertex* curve1, BranchModel::Vertex* curve2,
									 u16 count, float offset, const Vector3& scale)
{
	m_offset = offset;
	m_curve = curve1;
	m_curve2 = curve2;
	m_curveCount = count;
	m_curveScale = scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
