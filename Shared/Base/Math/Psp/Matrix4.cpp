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

#include "Matrix4.h"
#include "../../Serialize/Serializer.h"
#include "../../Io/Log.h"
#include "Trig.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Matrix4::serialize(Serializer& s, const char* name)
{
	s.beginGroup(SERIALIZER_NAME(name), SERIALIZER_NAME("zenic::Matrix4"));

	for (uint i = 0; i < (sizeof(m_floats)/sizeof(f32)); ++i)
		s << m_floats[i];

	s.endGroup();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4& Matrix4::makeXrotation(const float& angle)
{
	m_floats[(0<<2)+0] = 1;
	m_floats[(0<<2)+1] = 0;
	m_floats[(0<<2)+2] = 0;
	m_floats[(0<<2)+3] = 0;

	m_floats[(1<<2)+0] = 0;
	m_floats[(1<<2)+1] = zenic::cos(angle);
	m_floats[(1<<2)+2] = zenic::sin(angle);
	m_floats[(1<<2)+3] = 0;

	m_floats[(2<<2)+0] = 0;
	m_floats[(2<<2)+1] = -zenic::sin(angle);
	m_floats[(2<<2)+2] = zenic::cos(angle);
	m_floats[(2<<2)+3] = 0;

	m_floats[(3<<2)+0] = 0;
	m_floats[(3<<2)+1] = 0;
	m_floats[(3<<2)+2] = 0;
	m_floats[(3<<2)+3] = 1;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4& Matrix4::makeYrotation(const float& angle) 
{
	m_floats[(0<<2)+0] = zenic::cos(angle);
	m_floats[(0<<2)+1] = 0;
	m_floats[(0<<2)+2] = -zenic::sin(angle);
	m_floats[(0<<2)+3] = 0;

	m_floats[(1<<2)+0] = 0;
	m_floats[(1<<2)+1] = 1;
	m_floats[(1<<2)+2] = 0;
	m_floats[(1<<2)+3] = 0;

	m_floats[(2<<2)+0] = zenic::sin(angle);
	m_floats[(2<<2)+1] = 0;
	m_floats[(2<<2)+2] = zenic::cos(angle);
	m_floats[(2<<2)+3] = 0;

	m_floats[(3<<2)+0] = 0;
	m_floats[(3<<2)+1] = 0;
	m_floats[(3<<2)+2] = 0;
	m_floats[(3<<2)+3] = 1;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Matrix4& Matrix4::makeZrotation(const float& angle)
{
	m_floats[(0<<2)+0] = zenic::cos(angle);
	m_floats[(0<<2)+1] = zenic::sin(angle);
	m_floats[(0<<2)+2] = 0;
	m_floats[(0<<2)+3] = 0;

	m_floats[(1<<2)+0] = -zenic::sin(angle);
	m_floats[(1<<2)+1] = zenic::cos(angle);
	m_floats[(1<<2)+2] = 0;
	m_floats[(1<<2)+3] = 0;

	m_floats[(2<<2)+0] = 0;
	m_floats[(2<<2)+1] = 0;
	m_floats[(2<<2)+2] = 1;
	m_floats[(2<<2)+3] = 0;

	m_floats[(3<<2)+0] = 0;
	m_floats[(3<<2)+1] = 0;
	m_floats[(3<<2)+2] = 0;
	m_floats[(3<<2)+3] = 1;

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Matrix4::apply(const Vector3& vector) const
{
	Vector3 res;

	res.x = (vector.x * m_floats[(0<<2)+0]) + (vector.y * m_floats[(1<<2)+0]) + (vector.z * m_floats[(2<<2)+0]);
	res.y = (vector.x * m_floats[(0<<2)+1]) + (vector.y * m_floats[(1<<2)+1]) + (vector.z * m_floats[(2<<2)+1]);
	res.z = (vector.x * m_floats[(0<<2)+2]) + (vector.y * m_floats[(1<<2)+2]) + (vector.z * m_floats[(2<<2)+2]);

	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Stream& operator<<(Stream& stream, const Matrix4& m)
{
	const f32* matrix = m.matrix();

	for (uint i = 0; i < Matrix4::Rows; ++i)
	{
		for (uint j = 0; j < Matrix4::Columns; ++j)
			stream << matrix[(i*Matrix4::Columns)+j] << " ";

		stream << endl;
	}

	return stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
