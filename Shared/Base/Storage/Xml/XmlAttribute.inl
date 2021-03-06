inline XmlAttribute::XmlAttribute() : m_prev(0), m_next(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline XmlAttribute::XmlAttribute(const String& name) : m_name(name), m_prev(0), m_next(0)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const String& XmlAttribute::name() const
{
	return m_name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void XmlAttribute::setName(const String& name)
{
	m_name = name;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const String& XmlAttribute::value() const
{
	return m_value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void XmlAttribute::setValue(const String& value)
{
	m_value = value;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool XmlAttribute::toInteger(int& /*i*/)
{
	ZENIC_ASSERT(0);
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool XmlAttribute::toFloat(f32& /*i*/)
{
	ZENIC_ASSERT(0);
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline XmlDocument* XmlAttribute::document() const
{
	return m_document;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void XmlAttribute::setDocument(XmlDocument* document)
{
	m_document = document;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline XmlAttribute* XmlAttribute::prev() const
{
	return m_prev;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline XmlAttribute* XmlAttribute::next() const
{
	return m_next;
}
