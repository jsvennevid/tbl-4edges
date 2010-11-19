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
#ifndef zenic_Rtti_h
#define zenic_Rtti_h

#include "../Debug/Assert.h"

namespace zenic
{

class Rtti
{
public:

	Rtti(const Rtti* parent);

	const Rtti* parent() const;

private:

	const Rtti* m_parent;
};

#include "Rtti.inl"

}

#define ZENIC_RTTI \
	public: \
		static const zenic::Rtti ms_rtti; \
		virtual const zenic::Rtti* rtti() const;

#define ZENIC_ROOT_RTTI ZENIC_RTTI

#define ZENIC_CREATE_RTTI(curr, parent) \
		const zenic::Rtti type::ms_rtti(&parent::ms_rtti); \
		const zenic::Rtti* type::rtti() const { return &ms_rtti; }

#define ZENIC_CREATE_ROOT_RTTI(type) \
		const zenic::Rtti type::ms_rtti(0); \
		const zenic::Rtti* type::rtti() const { return &ms_rtti; }

#endif
