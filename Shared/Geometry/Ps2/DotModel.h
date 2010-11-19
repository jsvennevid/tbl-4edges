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

#ifndef zenic_ps2_DotModel_h
#define zenic_ps2_DotModel_h

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

class DotModel : public Model
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	DotModel();
	//virtual ~DotModel();

	void serialize(Serializer& s);

	const Vector3& scale() const;
	void setScale(const Vector3& scale);

	const DataPtr<s16>& points() const;
	void setPoints(const DataPtr<s16>& point);

private:

	DataPtr<s16> m_points;
	Vector3 m_scale;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Vector3& DotModel::scale() const
{
	return m_scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void DotModel::setScale(const Vector3& scale)
{
	m_scale = scale;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void DotModel::setPoints(const DataPtr<s16>& points)
{
	m_points = points;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
	}
}

#endif


