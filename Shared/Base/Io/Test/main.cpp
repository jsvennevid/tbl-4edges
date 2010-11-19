#include <Shared/Base/Log/Log.h>
#include <Shared/Base/Io/File.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ZENIC_PS2
#define PREFIX "host:../"
#else
#define PREFIX "../"
#endif

static const char* fileData = "1234567890";

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool verifyFile(const char* fileName)
{
	char temp[10];
	zenic::File file;

	if (!file.open(fileName, zenic::File::Read))
	{
		ZENIC_INFO("Unable to open testfile")
		return false;
	}

	if (file.read(reinterpret_cast<u8*>(&temp),10) != 10)
	{
		ZENIC_INFO("Failed to read from file")
		return false;
	}

	for (uint i = 0; i < 10; ++i)
	{
		if (fileData[i] != temp[i])
		{
			ZENIC_INFO("Data does not match at " << i);
			return false;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	zenic::File file;

	if (!verifyFile(PREFIX "Data/Data.txt"))
		return false;

	if (!file.open(PREFIX "Data/DataOut.txt", zenic::File::Write))
	{
		ZENIC_INFO("Unable to open Data/Out.txt")
		return false;
	}

	if (file.write(reinterpret_cast<const u8*>(fileData),10) != 10)
	{
		ZENIC_INFO("Failed to write to file")
		return false;
	}

	file.close();

	if (!verifyFile(PREFIX "Data/DataOut.txt"))
		return false;

	return true;
}
