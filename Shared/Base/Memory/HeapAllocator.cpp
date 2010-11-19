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
#include "HeapAllocator.h"

#include <malloc.h>
#include <stdio.h>

namespace zenic
{

HeapAllocator HeapAllocator::ms_allocator;

HeapAllocator::~HeapAllocator()
{
}

void* HeapAllocator::allocate(size_t size, size_t /*align*/)
{
#ifdef ZENIC_PS2
	return ::malloc(size);
#else
	return _aligned_malloc(size, 16);
#endif
}

void HeapAllocator::free(void* ptr)
{
#ifdef ZENIC_PS2
	printf("free ptr: 0x%x\n", (int)ptr);
	::free(ptr);
#else
	_aligned_free(ptr);
#endif
}

Allocator::Handle HeapAllocator::handle() const
{
	return 0;
}

}
