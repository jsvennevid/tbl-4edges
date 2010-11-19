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

void Matrix4::setScale(const Vector3& scale)
{
	m_floats[(0 << 2) + 0] = scale.x;
	m_floats[(1 << 2) + 1] = scale.y;
	m_floats[(2 << 2) + 2] = scale.z;
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
