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

#ifndef zenic_ps2_BranchModel_h
#define zenic_ps2_BranchModel_h

#include <Shared/Base/Math/Vector3.h>
#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class BranchModel : public Model
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	BranchModel();
	virtual ~BranchModel();

	void serialize(Serializer& s);

	struct Vertex
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
		s16 x, y, z;
	};

	const Vector3& scale() const;
	void setScale(const Vector3& scale);

	const Vector3& offset() const;
	void setOffset(const Vector3& offset);

	const DataPtr<Vertex>& curves() const;
	void setCurves(const DataPtr<Vertex>& curves);

	const DataPtr<u16>& curvesCount() const;
	void setCurvesCount(const DataPtr<u16>& curvesCount);

private:

	DataPtr<u16> m_curvesCount;
	DataPtr<Vertex> m_curves;
	Vector3 m_scale;
	Vector3 m_offset;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Vector3& BranchModel::scale() const
{
	return m_scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void BranchModel::setScale(const Vector3& scale)
{
	m_scale = scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Vector3& BranchModel::offset() const
{
	return m_scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void BranchModel::setOffset(const Vector3& offset)
{
	m_offset = offset;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const DataPtr<BranchModel::Vertex>& BranchModel::curves() const
{
	return m_curves;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void BranchModel::setCurves(const DataPtr<BranchModel::Vertex>& curves)
{
	m_curves = curves;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const DataPtr<u16>& BranchModel::curvesCount() const
{
	return m_curvesCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void BranchModel::setCurvesCount(const DataPtr<u16>& curvesCount)
{
	m_curvesCount = curvesCount;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
	}
}

#endif

