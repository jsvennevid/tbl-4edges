#ifndef zenic_SerializableArray_h
#define zenic_SerializableArray_h

namespace zenic
{

template <class T> class SerializableArray
{
public:
	SerializableArray(T* values, uint count) : m_values(values), m_count(count)
	{
	}

	T* values() const { return m_values; }
	uint count() const { return m_count; }

private:
	T* m_values;
	uint m_count;
};

}

#endif
