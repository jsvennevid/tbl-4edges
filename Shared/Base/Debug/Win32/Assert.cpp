///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2003-2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
