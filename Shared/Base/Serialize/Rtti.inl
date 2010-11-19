inline Rtti::Rtti(const Rtti* parent) : m_parent(parent)
{
}

inline const Rtti* Rtti::parent() const
{
	return m_parent;
}

template <class T, class N> bool isTypeOf(N* object)
{
	ZENIC_ASSERT(object);
	for (const zenic::Rtti* curr = object->rtti(); curr; curr = curr->parent())
	{
		if (&T::ms_rtti == curr)
			return true;
	}
	return false;
}

template <class T, class N> bool isExactTypeOf(N* instance)
{
	ZENIC_ASSERT(instance);
	return &T::ms_rtti == instance->rtti();
}

template <class T, class N> T* cast(N* object)
{
	if (object && isTypeOf(object))
		return static_cast<T*>(object);
	return 0;
}
