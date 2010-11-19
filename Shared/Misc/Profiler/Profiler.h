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

#ifndef zenic_Profiler_h
#define zenic_Profiler_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Timer/Timer.h>
#include <Shared/Base/Storage/String.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Profiler
{
public:
	
	Profiler(const char* name);
	virtual ~Profiler();
		
	static void init();
	static void stats();
		
private:

	enum
	{
		TimeBuffers = 16
	};

	struct Node
	{
		String name;
		u64 time;
		u64 accTime;
	
		u64 timeBuffer[TimeBuffers];
		u64 timeIndex;
	
		Node* parent;
		Node* children;
		Node* childTail;
		Node* next;
	};
	
	Node* branchStepDown(const char* name);
	void branchStepUp();
	static void executeTime(const Node* node, u64& time);
	static void printStatsRecurse(const Node* node, int dpt, uint& line, u64 time);

	u64 m_time0;
	u64 m_time1;
	
	const char* m_name;
	Node* m_curr;
	static Node* m_root;
	static Node* m_pos;

	static Timer m_timer;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#define ZENIC_PROFILER_SCOPE_BEGIN(name) { zenic::Profiler _profile##name(#name);
#define ZENIC_PROFILER_SCOPE_END() }

#endif // zenic_Matrix4_h

