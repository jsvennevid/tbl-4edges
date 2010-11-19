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

#ifndef zenic_win32_Matrix4_h
#define zenic_win32_Matrix4_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"
#include "Vector3.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Serializer;
	class Stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Matrix4
{
public:

	enum { Columns = 4, Rows = 4 };
	enum { X = 0, Y = 1, Z = 2, W = 3 };

	Matrix4();
	Matrix4(const Matrix4& m);

	void serialize(Serializer& s, const char* name = 0);

	Matrix4& operator = (const Matrix4& m);

	Matrix4 operator * (const Matrix4& m) const;
	Matrix4 operator - (void) const;

	Matrix4& makeXrotation(const float& angle);
	Matrix4& makeYrotation(const float& angle);
	Matrix4& makeZrotation(const float& angle);

	Vector3 apply(const Vector3& vector) const;
	void setScale(const Vector3& scale);

	Matrix4 transpose() const;
	Matrix4 fastInverse() const;

	static Matrix4 zero();
	static Matrix4 identity();

	const f32* matrix() const;
	f32* matrix();

	Vector3 x() const;
	Vector3 y() const;
	Vector3 z() const;
	Vector3 position() const;

	void setPosition(const Vector3& position);

	void multiplyBegin1(const Matrix4& m) const;
	void multiplyEnd1();

private:

	union
	{
		f32 m_floats[Columns * Rows];
		f32 m_matrix[Rows][Columns];
	};
} __attribute__((aligned(16)));

Stream& operator<<(Stream& stream, const Matrix4& m);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Matrix4.inl"

}

#endif //zenic_win32_Matrix4_h

