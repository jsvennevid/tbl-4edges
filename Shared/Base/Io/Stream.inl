inline Stream::Stream() : m_precision(6)
{
	resetManipulators();
}

inline Stream& Stream::operator >> (c8& value)
{
	read(&value,1);
	return *this;
}

inline Stream& Stream::operator >> (u8& value)
{
	value = static_cast<u8>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (s8& value)
{
	value = static_cast<s8>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (u16& value)
{
	value = static_cast<u16>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (s16& value)
{
	value = static_cast<s16>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (u32& value)
{
	value = static_cast<u32>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (s32& value)
{
	value = static_cast<s32>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (u64& value)
{
	value = static_cast<u64>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (s64& value)
{
	value = static_cast<s64>(readUint());
	return *this;
}

inline Stream& Stream::operator >> (f32& value)
{
	value = readFloat();
	return *this;
}

inline int Stream::size()
{
	int current = tell();
	seek(End, 0);
	int size = tell();
	seek(Start, current);

	return size;
}

inline void Stream::intToAscii(s64 value)
{
	uintToAscii(static_cast<u64>(value), m_hex, m_width, m_fillchar, true);
}

inline Stream& Stream::operator << (Stream& (*_f)(Stream&))
{
	return _f(*this);
}

inline Stream& Stream::operator << (DataPtr<u8> buffer)
{
	write(buffer.objects(), buffer.count());
	return *this;
}

template<typename Argument> inline Stream& Stream::operator << (const Manipulator<Argument>& manipulator)
{
	return manipulator.m_function(*this, manipulator.m_argument);
}

template<class T> inline bool Stream::read(DataPtr<T> buffer)
{
	return read(reinterpret_cast<u8*>(buffer.objects()), sizeof(T) * buffer.count());
}

template<class T> inline bool Stream::write(DataPtr<T> buffer)
{
	return uint(write(reinterpret_cast<const u8*>(buffer.objects()), sizeof(T) * buffer.count())) == (sizeof(T) * buffer.count()) ? true : false;
}

inline void Stream::resetManipulators()
{
	m_hex = false;
	m_precision = 6;
	m_width = 0;
	m_fillchar = ' ';
}

inline void Stream::setHex(bool hex)
{
	m_hex = hex;
}

inline void Stream::setPrecision(uint precision)
{
	m_precision = precision;
}

inline void Stream::setWidth(uint width)
{
	m_width = width;
}

inline void Stream::setFill(c8 fillchar)
{
	m_fillchar = fillchar;
}

inline Stream& _setprecision(Stream& stream, uint precision)
{
	stream.setPrecision(precision);
	return stream;
}

inline Stream::Manipulator<uint> setprecision(uint precision)
{
	return Stream::Manipulator<uint>(_setprecision, precision);
}

inline Stream& _setw(Stream& stream, uint width)
{
	stream.setWidth(width);
	return stream;
}

inline Stream::Manipulator<uint> setw(uint width)
{
	return Stream::Manipulator<uint>(_setw, width);
}

inline Stream& _setfill(Stream& stream, c8 fillchar)
{
	stream.setFill(fillchar);
	return stream;
}

inline Stream::Manipulator<c8> setfill(c8 fillchar)
{
	return Stream::Manipulator<c8>(_setfill, fillchar);
}

inline Stream& flush(Stream& stream)
{
	stream.flush();
	return stream;
}

inline Stream& endl(Stream& stream)
{
	return stream << '\n' << flush;
}

inline Stream& ends(Stream& stream)
{
	return stream << '\0';
}

inline Stream& hex(Stream& stream)
{
	stream.setHex(true);
	return stream;
}

inline Stream& dec(Stream& stream)
{
	stream.setHex(false);
	return stream;
}
