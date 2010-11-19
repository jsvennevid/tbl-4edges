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

#include "BranchGenerator.h"
#include "Branch.h"
#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Debug/Assert.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Branch BranchGenerator::m_branches[100];
Vector3 BranchGenerator::m_strips[(5*100) * 100];
int BranchGenerator::m_branchIndex = 0;
int BranchGenerator::m_stripIndex = 0;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Branch* BranchGenerator::generate(Branch* clone)
{
	if (m_branchIndex > 1000) 
		return 0;

	Branch* branch = &m_branches[m_branchIndex];
	branch->clone(clone);

	m_branchIndex++;

	return branch;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3* BranchGenerator::allocStrips()
{
	if (m_stripIndex > 2000)
		return 0;

	Vector3* strips = &m_strips[m_stripIndex * (5 * 100)];
	m_stripIndex++;

	return strips;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
