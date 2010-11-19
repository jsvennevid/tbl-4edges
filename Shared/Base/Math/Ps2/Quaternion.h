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

#ifndef zenic_win32_Quaternion_h
#define zenic_win32_Quaternion_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../Types.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

class Serializer;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Quaternion
{
public:

	Quaternion();
	Quaternion(f32 w, f32 x, f32 y, f32 z);
	Quaternion(const Quaternion& q);
	Quaternion(const Matrix4& m);
	Quaternion(const Vector3& axis, f32 angle);

	void serialize(Serializer& s, const char* name);

	void fromAxisAngle(const Vector3& axis, f32 angle);
	void fromMatrix4(const Matrix4& m);

	static Quaternion zero();
	static Quaternion identity();

	f32 w,x,y,z;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Quaternion.inl"

}

#endif // zenic_win32_Quaternion_h

