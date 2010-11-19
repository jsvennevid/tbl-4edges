#ifndef zenic_Base64_h
#define zenic_Base64_h

#include "String.h"

namespace zenic
{
	class Stream;
	class String;
}

namespace zenic
{

class Base64
{
public:

	template<class T> static void encode(Stream& stream, DataPtr<T> data);
	static void decode(Stream& stream, const String& data);

	static void encode(Stream& stream, const u8* data, uint size);
	static void decode(Stream& stream, const c8* data, uint size);

private:

	Base64();

	static const c8 m_alphabet[64];
	static const s8 m_reverse[256];
};

#include "Base64.inl"

}

#endif
