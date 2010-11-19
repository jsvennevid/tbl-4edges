///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2004 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

