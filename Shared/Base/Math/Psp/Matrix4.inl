///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4::Matrix4()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4::Matrix4(const Matrix4& m)
{
	const f32* src = m.m_floats;
	f32* dest = m_floats;

	for (uint i = 0; i < Columns * Rows; ++i)
		(*dest++) = (*src++);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4& Matrix4::operator = (const Matrix4& m)
{
	const f32* src = m.m_floats;
	f32* dest = m_floats;

	for (uint i = 0; i < Columns * Rows; ++i)
		(*dest++) = (*src++);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4 Matrix4::operator * (const Matrix4& m) const
{
	Matrix4 temp;

	const f32* m1 = m_floats;
	const f32* m2 = m.m_floats;
	f32* dest = temp.m_floats;

	for (uint i = 0; i < Rows; ++i)
	{
		for (uint j = 0; j < Columns; ++j)
		{
			f32 t = 0;

			for (uint k = 0; k < Columns; ++k)
				t += m1[(i * Columns) + k] * m2[(k * Columns) + j];

			(*dest++) = t;
		}
	}

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4 Matrix4::transpose() const
{
	Matrix4 temp;

	const f32* src = m_floats;
	f32* dest = temp.m_floats;

	for (uint i = 0; i < Columns; ++i)
	{
		(*dest++) = src[(0 * Columns) + i];
		(*dest++) = src[(1 * Columns) + i];
		(*dest++) = src[(2 * Columns) + i];
		(*dest++) = src[(3 * Columns) + i];
	}

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4 Matrix4::fastInverse() const
{
	Matrix4 temp;

	const f32* src = m_floats;
	f32* dest = temp.m_floats;

	// transpose rotation matrix

	for (uint i = 0; i < (Rows-1); ++i)
	{
		(*dest++) = src[(0 * Columns) + i];
		(*dest++) = src[(1 * Columns) + i];
		(*dest++) = src[(2 * Columns) + i];
		(*dest++) = 0;
	}

	// compute inverse of position

	Vector3 newPosition = -position();

	(*dest++) = newPosition.dot(x());
	(*dest++) = newPosition.dot(y());
	(*dest++) = newPosition.dot(z());
	(*dest++) = 1;

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4 Matrix4::zero()
{
	Matrix4 temp;

	f32* dest = temp.m_floats;

	for (uint i = 0; i < Rows; ++i)
	{
		(*dest++) = 0;
		(*dest++) = 0;
		(*dest++) = 0;
		(*dest++) = 0;
	}

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Matrix4 Matrix4::identity()
{
	Matrix4 temp;

	f32* dest = temp.m_floats;

	(*dest++) = 1;
	(*dest++) = 0;
	(*dest++) = 0;
	(*dest++) = 0;

	(*dest++) = 0;
	(*dest++) = 1;
	(*dest++) = 0;
	(*dest++) = 0;

	(*dest++) = 0;
	(*dest++) = 0;
	(*dest++) = 1;
	(*dest++) = 0;

	(*dest++) = 0;
	(*dest++) = 0;
	(*dest++) = 0;
	(*dest++) = 1;

	return temp;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline f32* Matrix4::matrix()
{
	return (f32*)&m_floats;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const f32* Matrix4::matrix() const
{
	return (const f32*)&m_floats;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Matrix4::x() const
{
	return Vector3(m_matrix[X][X],m_matrix[X][Y],m_matrix[X][Z]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Matrix4::y() const
{
	return Vector3(m_matrix[Y][X],m_matrix[Y][Y],m_matrix[Y][Z]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Matrix4::z() const
{
	return Vector3(m_matrix[Z][X],m_matrix[Z][Y],m_matrix[Z][Z]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Vector3 Matrix4::position() const
{
	return Vector3(m_matrix[W][X],m_matrix[W][Y],m_matrix[W][Z]);
}
