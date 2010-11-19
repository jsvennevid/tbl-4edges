// (C) 2004 by Khaled Daham, <khaled@w-arts.com>
// 
// Singleton
//

#include <iterator>
#include "Log.h"
#include <stdio.h>
#include <stdarg.h> 
#include <windows.h>

namespace ps2emu
{

typedef std::map<int32, std::string>::iterator m_mapIterator;

/////////////////////////////// PUBLIC ///////////////////////////////////////
Log*    Log::_instance = 0;

Log* Log::Instance() {
    if(_instance == 0) {
        _instance = new Log;
    }
    return _instance;
}

void
Log::Error(const std::string& message) 
{
	OutputDebugStringA(message.c_str());
}

void
Log::Error(const int32 errNumber) 
{
}

void
Log::Error(const int32 errNumber, const std::string& message) 
{
	OutputDebugStringA(message.c_str());
}

void
Log::Warning(const std::string& message)
{
    if ( !(message == *m_pLastMessage) ) 
	{
        m_numLastMessage++;
        return;
    }

	OutputDebugStringA(message.c_str());
	
	return;
}

void
Log::Warning(const int32 errNumber)
{
	return;
}

void
Log::Trace(const std::string& message) 
{
	//OutputDebugStringA(message.c_str());
	return;
}

void
Log::Trace(int32 level, const std::string& message) 
{
	if (!m_isTraceActive)
		return;

	switch(level) 
	{
        case 0:
            //m_pOut->AppendText(message);
            break;
        case 1:
            //m_pOut->AppendText("  " + message);
            break;
        case 2:
            //m_pOut->AppendText("    " + message);
            break;
        case 3:
            //m_pOut->AppendText("      " + message);
            break;
        default:
            //m_pOut->AppendText(message);
            break;
    }

	//OutputDebugStringA(message.c_str());
	return;
}

void
Log::SetTextCtrl(wxTextCtrl* out) {
    return;
}

std::string Log::Format(const char* fmt, ... )
{
    va_list args;
    char message[512];

    va_start(args, fmt);
    vsnprintf(message, 512, fmt, args);
    va_end(args);

	std::string out(message);
	return out;
}

/////////////////////////////// PRIVATE ///////////////////////////////////////
Log::Log() :
	m_isTraceActive(true),
	m_oldTraceState(true)
{
    m_numLastMessage = 0;
    m_pLastMessage = new std::string();

    object.insert(std::pair<int32, std::string>(0, ""));
    object.insert(std::pair<int32, std::string>(E_TIMEOUT,
            "Operation timed out"));
    object.insert(std::pair<int32, std::string>(E_NO_LINK,
            "No connection to ps2link server"));
    object.insert(std::pair<int32, std::string>(E_SOCK_CLOSE,
            "Connection reset by peer"));

    object.insert(std::pair<int32, std::string>(E_FILE_OPEN,
            "Unable to open file"));
    object.insert(std::pair<int32, std::string>(E_FILE_READ,
            "Unable to read from file"));
    object.insert(std::pair<int32, std::string>(E_FILE_WRITE,
            "Unable to write to file"));
    object.insert(std::pair<int32, std::string>(E_FILE_EOF,       "EOF reached"));

    object.insert(std::pair<int32, std::string>(E_VIF_DECODE,     "Bad VIF code"));
}

Log::~Log() {
}

}
