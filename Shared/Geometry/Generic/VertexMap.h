#ifndef zenic_generic_VertexMap_h
#define zenic_generic_VertexMap_h

#include <Shared/Base/Serialize/Serializable.h>
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

#include <Shared/Base/Storage/String.h>

namespace zenic
{

namespace generic
{

class VertexMap : public Serializable
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	enum Type
	{
		Float,
		Signed,
		Unsigned
	};

	enum Usage
	{
		Position,
		Normal,
		Tangent,
		Binormal,
		Weight,
		Color,
		Texcoord
	};

	VertexMap();
	virtual ~VertexMap();

	virtual void serialize(Serializer& s);

	void setName(const String& name);
	const String& name() const;

	void setType(Type type);
	Type type() const;

	void setUsage(Usage usage);
	Usage usage() const;

	void setDimensions(u32 dimensions);
	u32 dimensions() const;

	void setNext(VertexMap* next);
	VertexMap* next() const;

	void setIndices(DataPtr<u32> indices);
	DataPtr<u32> indices() const;

	void setFloats(DataPtr<f32> floats);
	DataPtr<f32> floats() const;

	void setIntegers(DataPtr<u32> integers);
	DataPtr<u32> integers() const;

private:

	class Index
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		u32 m_index;
	};

	class FloatData
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		f32 m_value;
	};

	class IntegerData
	{
		ZENIC_SERIALIZABLE_STRUCTURE;
	public:
		u32 m_value;
	};

	String m_name;
	u32 m_type;
	u32 m_usage;
	u32 m_dimensions;

	VertexMap* m_next;

	DataPtr<Index> m_indices;
	DataPtr<FloatData> m_floats;
	DataPtr<IntegerData> m_integers;
};

#include "VertexMap.inl"

}

}

#endif
