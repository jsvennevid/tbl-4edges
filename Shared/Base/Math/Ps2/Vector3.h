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

#ifndef zenic_win32_Vector3_h
#define zenic_win32_Vector3_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"
#include "Trig.h"
#include <math.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Forwards
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Serializer;
	class Log;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Vector3
{
public:

	Vector3();
	Vector3(const Vector3& v);
	Vector3(f32 x, f32 y, f32 z);

	void serialize(Serializer& s, const char* name = 0);

	Vector3& operator = (const Vector3& v);

	Vector3 operator + (const Vector3& v) const;
	Vector3 operator - (const Vector3& v) const;
	Vector3 operator * (float f) const;
	Vector3 operator - (void) const;

	Vector3& operator += (const Vector3& v);
	Vector3& operator -= (const Vector3& v);
	Vector3& operator *= (float f);

	float length() const;
	Vector3 normalize() const;
	Vector3 cross(const Vector3& v) const;
	f32 dot(const Vector3& v) const;

	f32 x,y,z;
};

Log& operator<<(Log& stream, const Vector3& v);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Vector3.inl"

}

#endif //zenic_win32_Vector3_h
