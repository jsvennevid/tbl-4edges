template<class T> void Base64::encode(Stream& stream, DataPtr<T> data)
{
	encode(stream,reinterpret_cast<const u8*>(data.objects()),data.count() * sizeof(T));
}

inline void Base64::decode(Stream& stream, const String& data)
{
	decode(stream,data.contents(),data.length());
}
