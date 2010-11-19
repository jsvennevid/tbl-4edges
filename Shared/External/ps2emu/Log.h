// (C) 2004 by Khaled Daham, <khaled@w-arts.com>
// 
// Singleton
//
#ifndef __LOG__
#define __LOG__

#include <map>
#include <string>
#include "datatypes.h"

namespace ps2emu
{

class wxFrame;
//class wxstd::string;
class wxTextCtrl;

const int32     E_OK            =   0;
const int32     E_TIMEOUT       =   -10;
// VU Errors

// PS2Link protocol errorrs 
const int32     E_SOCKET        =   -20;
const int32     E_NO_LINK       =   -21;
const int32     E_LINK_TIMEOUT  =   -22;
const int32     E_SOCK_CLOSE    =   -23;

// File operations errors
const int32     E_FILE_OPEN     =   -30;
const int32     E_FILE_READ     =   -31;
const int32     E_FILE_WRITE    =   -32;
const int32     E_FILE_EOF      =   -33;

//
const int32     E_SYSTEM_CMD    =   -40;

// VIF Errorsx
const int32     E_VIF_DECODE    =   -50;

class Log {
public:
    static Log*     Instance();
    void			SetFrame(wxFrame* frame);
    void			SetTextCtrl(wxTextCtrl* out);
	void			SetTraceActive(bool state);
	void			RestoreTraceState();
	void            Error(const std::string& message);
    void            Error(const int32 errNumber);
	void            Error(const int32 errNumber, const std::string& message);
    void            Warning(const std::string& message);
    void            Warning(const int32 errNumber);
    void            Trace(const std::string& message);
	void            Trace(int32 level, const std::string& message);
	static std::string Format(const char* fmt, ... );
protected:
    Log();
    ~Log();
private:
    static Log*     _instance;
	std::string*    m_pLastMessage;
    uint32          m_numLastMessage;
	std::map<int32, std::string> object;
	bool m_isTraceActive:1;
	bool m_oldTraceState:1;
};

inline void Log::SetTraceActive(bool state)
{
	m_oldTraceState = m_isTraceActive;
	m_isTraceActive = state;
}

inline void Log::RestoreTraceState()
{
	m_isTraceActive = m_oldTraceState;
}

}

#endif
