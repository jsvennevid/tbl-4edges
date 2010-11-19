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
#ifndef zenic_dx9_ModelData_h
#define zenic_dx9_ModelData_h

#include "../ModelData.h"

#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

namespace zenic
{

namespace dx9
{

class ModelData : public zenic::ModelData
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	class Stream
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:

		Stream();

		void setType(u32 type);
		u32 type() const;

		void setMethod(u32 method);
		u32 method() const;

		void setUsage(u32 usage);
		u32 usage() const;

		void setIndex(u32 index);
		u32 index() const;

		void setSize(u32 size);
		u32 size() const;

		void setData(DataPtr<u8> data);
		DataPtr<u8> data() const;

	private:
		u32 m_type;
		u32 m_method;
		u32 m_usage;

		u32 m_index;
		u32 m_size;

		DataPtr<u8> m_data;
	};

	ModelData();
	virtual ~ModelData();

	virtual void serialize(Serializer& s);

	void setType(u32 type);
	u32 type() const;

	void setNumVertices(u32 numVertices);
	u32 numVertices() const;

	void setNumPrimitives(u32 numPrimitives);
	u32 numPrimitives() const;

	void setNumStreams(u32 streams);
	u32 numStreams() const;

	void setStream(u32 index, Stream& stream);
	Stream& stream(u32 index) const;

	void setIndexFormat(u32 indexFormat);
	u32 indexFormat() const;

	void setIndices(DataPtr<u8> indices);
	DataPtr<u8> indices() const;

private:

	u32 m_type;
	u32 m_numVertices;
	u32 m_numPrimitives;

	DataPtr<Stream> m_streams;

	u32 m_indexFormat;
	DataPtr<u8> m_indices;
};

#include "ModelData.inl"

}

}


#endif
