///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Timer.h"
#include "../../Debug/Assert.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer::reset()
{
	// setup high-resolution timer

	LARGE_INTEGER freq;
	LARGE_INTEGER base;

	BOOL success = QueryPerformanceFrequency(&freq);
	ZENIC_ASSERT_DESC(success, "Failed querying for timer frequency");
	success = false;
	QueryPerformanceFrequency(&base);

	m_frequency = freq.QuadPart;	
	m_base = base.QuadPart;

	// initialize timer

	m_deltaTick = 0;
	m_tick = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Timer::update()
{
	u64 newTick = rawTick();

	m_deltaTick = newTick - m_tick;
	m_tick = newTick;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

u64 Timer::rawTick() const
{
	LARGE_INTEGER value;

	QueryPerformanceCounter(&value);

	return u64(((value.QuadPart-m_base) * u64(Resolution)) / m_frequency);
//	return u64(((long double)(value.QuadPart-m_base)/(long double)(m_frequency)) * ((long double)Resolution));
}

}
