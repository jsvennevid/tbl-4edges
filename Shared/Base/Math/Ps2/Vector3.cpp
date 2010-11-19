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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Vector3.h"
#include "../../Serialize/Serializer.h"
#include "../../Io/Log.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Vector3::serialize(Serializer& s, const char* name)
{
	s.beginGroup(SERIALIZER_NAME(name), SERIALIZER_NAME("zenic::Vector3"));

	s.descriptor(SERIALIZER_NAME("x")) << x;
	s.descriptor(SERIALIZER_NAME("y")) << y;
	s.descriptor(SERIALIZER_NAME("z")) << z;

	s.endGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Log& operator<<(Log& stream, const Vector3& v)
{
	stream << v.x << " " << v.y << " " << v.z;
	return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
