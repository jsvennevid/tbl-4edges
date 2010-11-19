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

// Notice: this code needs to be improved to handle things like
// -iImage.tga (argument directly after parameter)
// -i "my image here" (get string inside " ")

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Io/Log.h>
#include "Arguments.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Arguments::Arguments()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Arguments::~Arguments()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Arguments::init(int argc,char* argv[])
{
	if (argc == 1)
	{
		showHelp();
		return false;
	}

	for (uint i = 1; i < (uint)argc; ++i)
	{
		std::map<std::string,const char*>::iterator p = m_arguments.find(argv[i]);

		if (p != m_arguments.end())
			return false;

		m_arguments.insert(std::pair<std::string,const char*>(std::string(argv[i]), argv[i+1]));
		++i; // nice
	}
		
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const char* Arguments::getArg(const char* arg)
{
	std::map<std::string,const char*>::iterator i = m_arguments.find(arg);

	if (i != m_arguments.end())
		return i->second;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Arguments::showHelp()
{
	logOut << "Builder Parameters" << endl;
	logOut << "Notice: this code needs to be improved to handle things like" << endl;
	logOut << "-iImage.tga (argument directly after parameter)" << endl;
	logOut << "-i \"my image here\" (get string inside \" \")" << endl << endl;

	logOut << "-h Show this help" << endl;
	logOut << "-p [Platform] (ps2, opengl)" << endl;
	logOut << "-f texture format (4, 8, 32)" << endl;
	logOut << "-s Input Scene" << endl;
	logOut << "-i Input Image" << endl;
	logOut << "-o Output file" << endl;

	logOut << "Example: Builder -i myImage.jpg -o myImage.zei" << endl;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
