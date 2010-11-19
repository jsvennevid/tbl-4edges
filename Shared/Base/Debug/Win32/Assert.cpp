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

#include "../../Io/BufferStream.h"
#include "Assert.h"

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Assert::ms_methods = Assert::Requester|Assert::ErrorStream|Assert::DebugOutput;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Assert::assertStatement(const char* expression, const char* file, uint line)
{
	char progname[MAX_PATH + 1];
	BufferStream s;

	s << "Assertion failed!\n\n";
	s << "Program: ";

	progname[MAX_PATH] = '\0';
	if ( !GetModuleFileName(NULL, progname, sizeof(progname)))
		s << "<Program name unknown>";

	truncate(s, LineLength-9, progname);

	s << endl << "File: ";
	truncate(s, LineLength-6, file);

	s << endl << "Line: ";
	s << line;

	s << endl << "Expression: ";
	truncate(s, LineLength - 12, expression);

	s << endl;

	if (ms_methods & Assert::ErrorStream)
		fputs(s.string(), stderr);

	if (ms_methods & Assert::DebugOutput)
		OutputDebugString( s.string());

	if (ms_methods & Assert::Requester)
	{
		s << endl << "Press 'Retry' to debug application";

		int result = MessageBox(NULL, s.string(), "Cell Assertion Failed", 
								 MB_ABORTRETRYIGNORE|MB_ICONHAND|MB_SETFOREGROUND|MB_TASKMODAL);

		switch (result)
		{
			case IDABORT:
			{
				raise(SIGABRT);
				exit(3);
			}
			break;

			case IDRETRY:
			{
				DebugBreak();
				return;
			}
			break;

			case IDIGNORE:
				return;

			default:
				abort();
		}
	}

	raise(SIGABRT);
	exit(3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Assert::truncate(BufferStream& s, uint length, const char* input)
{
	if (input)
	{
		uint inlen = uint(strlen(input));
		if (inlen > length)
		{
			s << "...";
			s << input + (inlen-(length-4));
		}
		else
			s << input;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Assert::setMethods(uint methods)
{
	ms_methods = methods;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint Assert::methods()
{
	return ms_methods;
}

}
