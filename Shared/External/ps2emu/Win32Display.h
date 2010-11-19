#ifndef ps2emu_Display_h
#define ps2emu_Display_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "datatypes.h"
#include "GsRenderer.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ps2emu
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Display
{
public:
	Display();
	~Display();

	bool create(uint width, uint height, bool fullScreen = false, HINSTANCE instance = 0, HWND parent = 0);
	void destroy();
	bool update();

	bool isRunning() const;

	static float s_mouseDeltaX;
	static float s_mouseDeltaY;
	static bool s_isMovingMouse;
	static bool s_isMovingPos;

private:

	static LONG WINAPI wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
	bool setupDisplay();
	void initView();
	void resize(uint width, uint height);
	void makeCurrent();

	GsRenderer m_renderer;

	HDC m_hdc;
	HWND m_hwnd;
	HGLRC m_glrc;

	uint m_width;
	uint m_height;

	u32 m_fullScreen:1;

	static bool m_state;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Display::isRunning() const
{
	return m_state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
