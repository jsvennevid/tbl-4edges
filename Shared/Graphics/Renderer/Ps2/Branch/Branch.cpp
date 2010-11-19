///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-200 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Branch.h"
#include "BranchGenerator.h"
#include "BranchSettings.h"
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Math/Ps2/Trig.h>
//#include <Shared/Base/Math/Constants.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Io/Log.h>
#include <Shared/Hardware/Ps2/Vu1.h>
#include <Shared/Hardware/Ps2/Gif.h>
#include <string.h>

#include <Shared/Graphics/Renderer/Ps2/Renderer.h>
#include <Shared/Geometry/Ps2/BranchModel.h>
#include <Shared/Base/Memory/Prefetch.h>

#ifdef PS2_EMU
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef PS2_EMU
extern "C" u32 Render_Branch_CodeStart __attribute__((section(".vudata")));
extern "C" u32 Render_Branch_CodeEnd __attribute__((section(".vudata")));
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Branch::m_seed = 93186752;
BranchSettings* Branch::s_settings = 0;

float Branch::s_generateTime = 0.1f;
float Branch::s_splitTime = 0.0f;

float Branch::s_cosTable[MaxSides+1];
float Branch::s_sinTable[MaxSides+1];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Branch::Branch() : 
	m_split1(0), 
	m_split2(0), 
	m_curve(0),
	m_curve2(0),
	m_section(0),
	m_curveCount(0),
	m_curveTarget(1)
{
	m_strips = BranchGenerator::allocStrips();
	m_topSection = m_strips;
	m_section = 0;
	m_radius = 0.05f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Branch::Branch(const Branch* clone) : m_split1(0), m_split2(0)
{
	m_strips = zenic_new Vector3[(MaxSides + 1) * 20];

	memcpy(m_strips, clone->m_topSection, sizeof(Vector3) * s_settings->m_sides);
	m_position = clone->m_position;
	m_topSection = m_strips;
	m_direction = clone->m_direction;
	m_radius = clone->m_radius;

	m_curve = clone->m_curve;
	m_curveCount = clone->m_curveCount;
	m_curveTarget = clone->m_curveTarget;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::clone(const Branch* clone)
{
	m_strips = BranchGenerator::allocStrips();

	if (!m_strips)
	{
		m_topSection = 0;
		return;
	}

	memcpy(m_strips, clone->m_topSection, sizeof(Vector3) * s_settings->m_sides);
	m_position = clone->m_position;
	m_topSection = m_strips;
	m_direction = clone->m_direction;
	m_radius = clone->m_radius;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::split()
{
	if (m_split1)
	{
		m_split1->split();
		m_split2->split();
		return;
	}

	m_split1 = BranchGenerator::generate(this);
	m_split2 = BranchGenerator::generate(this);

	//m_split1->makeNewDirection(12.0f);
	//m_split2->makeNewDirection(12.0f);

	if (m_split1) 
	{
		m_split1->divide(-1.0f);
		m_split2->divide(1.0f);
	}
}

#ifdef PS2_EMU
float sqrt(float value) { return ::sqrt(value); }
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::divide(float angleMul, bool hasReachedTarget)
{
	if (m_split1)
	{
		m_split1->divide();
		m_split2->divide();
		return;
	}

	if (!makeNewDirection(angleMul, hasReachedTarget))
		return;

	m_section++;
	m_topSection += s_settings->m_sides;

	generateSection(m_topSection, m_position, m_position + m_direction);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Branch::makeNewDirection(float angleMul, bool hasReatchedTarget)
{
	if (m_curve)
	{
		if (m_curveTarget == m_curveCount)
			return false;

		Vector3 pos = curvePos();
		Vector3 direction = pos - m_position;

		// rand...

		float x = (s_settings->m_angle + s_settings->m_angleVariation * floatRandom()) * angleMul * 0.005f;
		float y = (s_settings->m_angle + s_settings->m_angleVariation * floatRandom()) * angleMul * 0.005f;
		float z = (s_settings->m_angle + s_settings->m_angleVariation * floatRandom()) * angleMul * 0.005f;

		direction = direction.normalize();
		direction.x += x;
		direction.y += y;
		direction.z += z;
		m_direction = direction.normalize();

		// Set next target if we reatched our target point in the curve

		if (hasReatchedTarget)
			++m_curveTarget;

		return true;
	}



	Matrix4 matrix;

	float angle = (s_settings->m_angle + s_settings->m_angleVariation * floatRandom()) * angleMul;

	if (random() & 1)
		angle = -angle;

	switch (random() %3 )
	{
		case 0 :
		{
			matrix.makeXrotation(angle * (3.1415f / 180.0f));
			//m_direction.y = 1.0f;
			break;
		}

		case 1 :
		{
			matrix.makeYrotation(angle * (3.1415f / 180.0f));
			//m_direction.z = 1.0f;
			break;
		}

		case 2 :
		{
			matrix.makeZrotation(angle * (3.1415f / 180.0f));
			//m_direction.x = 1.0f;
			break;
		}

		default : ZENIC_ASSERT(false);
	}

	m_direction = matrix.apply(m_direction);
	m_direction.y += -s_settings->m_gravity;
	m_direction = m_direction.normalize();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::generateSection(Vector3* coords, const Vector3& start, const Vector3& end)
{
	ZENIC_ASSERT(coords);

	Vector3 up(0.0f, 1.0f, 0.0f);
	Vector3 rel = end - start;
	Vector3 n = rel.normalize();

	Vector3 right = up.cross(n);
	float rightLength = right.length();

	if (rightLength)
		right *= 1.0f / rightLength;
	else
		right = Vector3(1.0f, 0, 0);

	up = n.cross(right);

	for (uint i = 0; i < s_settings->m_sides; ++i) 
	{
		Vector3 temp = up * (s_cosTable[i] * m_radius) + right * (s_sinTable[i] * m_radius);
		coords[i] = start + temp;
	}

	m_radius = m_radius * (s_settings->m_contact * 0.01f) + (s_settings->m_contactVariation * 0.01f * floatRandom());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::init()
{
	ZENIC_ASSERT(s_settings);
	ZENIC_ASSERT(s_settings->m_sides < MaxSides);
	float angle = 0.0f;
	const float angleStep = (3.1415f * 2.0f) / (s_settings->m_sides-1);

	for (uint i = 0; i < s_settings->m_sides; angle += angleStep, ++i) 
	{
		s_cosTable[i] = zenic::cos(angle);
		s_sinTable[i] = zenic::sin(angle);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::createRoot(const Vector3& orgin, const Vector3& direction)
{
	m_radius = (s_settings->m_startRadius) + (s_settings->m_startRadiusVariation * floatRandom());
	m_position = orgin;
	generateSection(m_strips, m_position, direction);
	m_position.y += 0.1f;

	if (m_curve)
		makeNewDirection();
	else
		m_direction = direction;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::createRoot()
{
	m_radius = (s_settings->m_startRadius) + (s_settings->m_startRadiusVariation * floatRandom());
	m_position = curvePos();
	generateSection(m_strips, m_position, Vector3(0.0f, 0.01f, 0.0f));
	m_position.y += 0.1f;
	makeNewDirection();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

s32 Branch::beginRender(Dma& dmaChain)
{
	s32 start = 0;
#ifndef PS2_EMU
	Vu1::instance()->clearCache();
	start = Vu1::instance()->getProgram(dmaChain, ps2::Vu1::BranchRenderer, 
										&Render_Branch_CodeStart, 
										&Render_Branch_CodeEnd);
#endif
	dmaChain.addSrcCntTag();

	dmaChain.add32(0);
	dmaChain.add32(0);

	dmaChain.add32(VIF_STCYCL(1, 1));
	dmaChain.add32(VIF_UNPACK(vif::V4_32, 2,  22));

	dmaChain.add64(GIFTAG(0, 0, gif::Pre, GS_PRIM(gs::Tristrip, gs::Gouraud, 0, 0, 0, 0, gs::Stq, gs::Context1, 0), 
						  gif::Packed, 1));
	dmaChain.add64(0x5);

	dmaChain.add64(GIFTAG(0, 0, gif::Pre, GS_PRIM(gs::Tristrip, gs::Gouraud, 0, 0, 0, 0, gs::Stq, gs::Context1, 0), 
						  gif::Packed, 1));
	dmaChain.add64(0x5);

	dmaChain.add32(VIF_BASE(24));
	dmaChain.add32(VIF_OFFSET(524));
	dmaChain.add32(0);
	dmaChain.add32(VIF_DIRECT(2));
	dmaChain.add128(GIFTAG(1, gif::Eop, 0, 0, gif::Packed, 1), gif::A_D);
	dmaChain.add128(GS_RGBAQ(0, 0, 0, 0, 0.0f), GS_REG(RGBAQ));

	dmaChain.endPacket();

	return start;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::render(Dma& dmaChain, float deltaTime)
{
	if (m_topSection)
		update(deltaTime, false);

	int sectionCount = (int)m_section;
	Vector3* strips = m_strips;

	if (!m_strips)
		return;

	while (sectionCount > 0)
	{
		int currentSectionCount = sectionCount;

		// lets say max 4 sections per chain

		if (currentSectionCount > 4)
			currentSectionCount = 4;

		const uint size = currentSectionCount * s_settings->m_sides * 2;

		dmaChain.addSrcCntTag();
		dmaChain.addItop(currentSectionCount);
		dmaChain.addStcycl(1,1);
		dmaChain.add32(0);
		dmaChain.add32(VIF_UNPACKR(vif::V3_32, size, 0));

		for (int k = 0; k < currentSectionCount; ++k)
		{
#ifdef PS2_EMU
			glBegin(GL_TRIANGLE_STRIP);
			glColor3f(0.0f, 0.0f, 0.0f);
#endif
			for (u16 i = 0; i < s_settings->m_sides; ++i)
			{
				dmaChain.addFloat(strips[i].x);
				dmaChain.addFloat(strips[i].y);
				dmaChain.addFloat(strips[i].z);

				dmaChain.addFloat(strips[i+s_settings->m_sides].x);
				dmaChain.addFloat(strips[i+s_settings->m_sides].y);
				dmaChain.addFloat(strips[i+s_settings->m_sides].z);
#ifdef PS2_EMU
				glVertex3f(strips[i].x, strips[i].y, strips[i].z);
				glVertex3f(strips[i+s_settings->m_sides].x, strips[i+s_settings->m_sides].y, strips[i+s_settings->m_sides].z);
#endif
			}

#ifdef PS2_EMU
			glEnd();
#endif
			strips += s_settings->m_sides;
		}


		dmaChain.addMscnt();
		//dmaChain.add32(VIF_NOP());

		dmaChain.align(16);
		dmaChain.endPacket();

		sectionCount -= currentSectionCount;
	}

	if (m_split1)
	{
		m_split1->render(dmaChain, deltaTime);
		m_split2->render(dmaChain, deltaTime);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Branch::update(float deltaTime, bool recursive)
{
	if (!m_split1)
	{
		//m_direction = m_direction.normalize();
		const Vector3 delta = m_direction * (s_settings->m_speed * deltaTime);

		m_position += delta;

		if (m_curve)
		{
			Vector3 pos = curvePos();
			Vector3 diff = pos - m_position;

			float dist = diff.dot(diff);

			if (dist < 0.2f)
			{
				divide(1.0f, true);
				return;
			}
		}

		for (u16 i = 0; i < s_settings->m_sides; ++i)
			m_topSection[i] += delta;
	}

	if (m_split1 && recursive)
	{
		m_split1->update(deltaTime, recursive);
		m_split2->update(deltaTime, recursive);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Branch::curvePos()
{
	Vector3 pos1(m_curve[m_curveTarget].x * m_curveScale.x,
				 m_curve[m_curveTarget].y * m_curveScale.y,
				 m_curve[m_curveTarget].z * m_curveScale.z);

	Vector3 pos2(m_curve2[m_curveTarget].x * m_curveScale.x,
				 m_curve2[m_curveTarget].y * m_curveScale.y,
				 m_curve2[m_curveTarget].z * m_curveScale.z);

	Vector3 pos(pos1.x + ((pos2.x - pos1.x) * m_offset),
				pos1.y + ((pos2.y - pos1.y) * m_offset),	
				pos1.z + ((pos2.z - pos1.z) * m_offset));

	return pos;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float Branch::floatRandom()  
{
	static uint a = 1588635695, m = 4294967291U, q = 2, r = 1117695901;
	m_seed = a*(m_seed % q) - r*(m_seed / q);
	return ((float)m_seed / (float)m);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Branch::random()  
{
	static uint a = 1588635695, q = 2, r = 1117695901;
	m_seed = a*(m_seed % q) - r*(m_seed / q);
	return m_seed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}


