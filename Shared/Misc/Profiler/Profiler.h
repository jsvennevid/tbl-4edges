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

