#ifndef zenic_Stream_h
#define zenic_Stream_h

#include "../Types.h"
#include "../Memory/DataPtr.h"

namespace zenic
{
	class String;
}

namespace zenic
{

class Stream
{
public:

	enum SeekMode
	{
		Start,
		Current,
		End
	};

	template<typename Argument> class Manipulator
	{
	public:

		typedef Stream& (*Function)(Stream&,Argument);

		Manipulator(Function function, Argument argument) : m_function(function),m_argument(argument)
		{
		}

		Function m_function;
		Argument m_argument;
	};

	Stream();
	virtual ~Stream();

	virtual int read(void* buffer, int size) = 0;
	virtual int write(const void* buffer, int size) = 0;
	virtual int seek(SeekMode mode, int offset) = 0;
	virtual bool flush() = 0;
	virtual int tell() const = 0;

	bool copy(Stream& stream, int size);
	int size();

	Stream& operator << (const void* pointer);
	Stream& operator << (const c8* string);

	Stream& operator << (c8 value);
	Stream& operator << (u8 value);
	Stream& operator << (s8 value);

	Stream& operator << (u16 value);
	Stream& operator << (s16 value);

	Stream& operator << (u32 value);
	Stream& operator << (s32 value);

	Stream& operator << (u64 value);
	Stream& operator << (s64 value);

	Stream& operator << (f32 value);

	Stream& operator << (Stream& (*_f)(Stream&));

	Stream& operator << (DataPtr<u8> buffer);

	Stream& operator >> (String& string);

	Stream& operator >> (c8& value);
	Stream& operator >> (u8& value);
	Stream& operator >> (s8& value);

	Stream& operator >> (u16& value);
	Stream& operator >> (s16& value);

	Stream& operator >> (u32& value);
	Stream& operator >> (s32& value);

	Stream& operator >> (u64& value);
	Stream& operator >> (s64& value);

	Stream& operator >> (f32& value);

	template<typename Argument> Stream& operator << (const Manipulator<Argument>& manipulator);

	template<class T> bool read(DataPtr<T> buffer);
	template<class T> bool write(DataPtr<T> buffer);

	void setHex(bool hex);
	void setPrecision(uint precision);
	void setWidth(uint width);
	void setFill(c8 fillchar);

	bool write32BE(u32 value);
	bool read32BE(u32& value);

private:

	enum { CopySize = 4096 };

	void resetManipulators();
	void generatePattern(char* output, char* input, uint precision = 0);

	// these are only present to catch attempts to use doubles, and are not to be implemented
	Stream& operator << (double);
	Stream& operator << (long double);
	
	u64 readUint();
	f32 readFloat();

	void uintToAscii(u64 value, bool hex, uint width, char fillchar, bool negative);
	void intToAscii(s64 value);
	void floatToAscii(f32 value, uint precision);

	static f32 modf(float value, f32* intptr);
	static f32 floor(float value);

	bool m_hex;
	uint m_precision;
	uint m_width;
	c8 m_fillchar;
};

#include "Stream.inl"

}

#endif
