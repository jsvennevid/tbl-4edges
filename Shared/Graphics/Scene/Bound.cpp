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

#include "Bound.h"
#include <Shared/Base/Serialize/Serializer.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Bound::serialize(Serializer& s, const char* name)
{
	s.beginGroup(name, SERIALIZER_NAME("zenic::Bound"));
	m_position.serialize(s, SERIALIZER_NAME("m_position"));
	s.descriptor("m_radius") << m_radius;
	s.endGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Bound Bound::merge(const Bound& bound) const
{
/*
	Vector3 direction = (bound.position()-position()).normalize();
	Vector3 extent1 = position() - (direction * radius());
	Vector3 extent2 = bound.position() + (direction * bound.radius());

	Vector3 radius = (extent2-extent1) * 0.5f;
	return Bound(extent1 + radius, sqrt(radius.dot(radius)));
*/
	return bound;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
