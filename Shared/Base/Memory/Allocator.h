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
#ifndef zenic_Allocator_h
#define zenic_Allocator_h

#include "../Types.h"
#include "New.h"

#include <new>

namespace zenic
{

class Allocator
{
public:

	Allocator();
	virtual ~Allocator();

	typedef u32 Handle;

	virtual void* allocate(size_t size, size_t align = 0) = 0;
	virtual void free(void* ptr) = 0;

	virtual Handle handle() const = 0;

	static Allocator* find(Handle handle);

	Allocator* next() const;
	static Allocator* first();

private:

	Allocator* m_next;
	static Allocator* ms_first;
};

#include "Allocator.inl"

#define ALLOCATOR_HANDLE(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|(d))

}

#endif
