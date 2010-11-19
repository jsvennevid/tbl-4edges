///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2006 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
