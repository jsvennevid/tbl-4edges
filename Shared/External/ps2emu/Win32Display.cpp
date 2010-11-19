#if defined(_MSC_VER)
#pragma warning(disable : 4312) // 'type cast' : conversion from 'LONG' to 'ps2emu::Display *' of greater size 
#pragma warning(disable : 4311) // type cast' : pointer truncation from 'LPVOID' to 'LONG' 
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include "Callbacks.h"
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ps2emu
{

bool Display::m_state = false;
float Display::s_mouseDeltaX = 0.0f;
float Display::s_mouseDeltaY = 0.0f;
bool Display::s_isMovingMouse = false;
bool Display::s_isMovingPos = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Display::Display() : m_renderer(true)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Display::~Display()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Display::create(uint width, uint height, bool fullScreen, HINSTANCE instance, HWND parent)
{
	RECT windowRect;										
	WNDCLASS wndclass; 

	m_width = width;
	m_height = height;
	m_fullScreen = fullScreen;

	windowRect.right = windowRect.left = windowRect.bottom = windowRect.top = 0;

	if (!instance)
		instance = GetModuleHandle(0); 

	wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; 
	wndclass.lpfnWndProc   = (WNDPROC)wndProc; 
	wndclass.cbClsExtra    = 0; 
	wndclass.cbWndExtra    = 0; 
	wndclass.hInstance     = instance; 
	wndclass.hIcon         = 0;
	wndclass.hCursor       = 0; 
	wndclass.hbrBackground = 0; 
	wndclass.lpszMenuName  = "ps2emu OpenGL Display"; 
	wndclass.lpszClassName = "ps2emu OpenGL Display"; 

	if (!RegisterClass(&wndclass)) 
		return false; 

	HWND hWnd;

	if (parent)
	{
		GetClientRect(parent, &windowRect);
		AdjustWindowRectEx(&windowRect, WS_CHILD, FALSE, 0);	

		hWnd = CreateWindowEx(0, "ps2emu OpenGL Display", "ps2emu OpenGL Display", 
							  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
							  0, 0, windowRect.right - windowRect.left,	windowRect.bottom - windowRect.top, 
							  parent, 0, instance, (LPVOID)this); 
	}
	else
	{
		hWnd = CreateWindow("ps2emu OpenGL Display", "ps2emu OpenGL Display", WS_OVERLAPPEDWINDOW, 0, 0, 
							m_width, m_height, 0, 0, instance, (LPVOID)this); 
	}

	m_hwnd = hWnd;
	m_hdc = GetDC(hWnd);

	setupDisplay();

	m_glrc = wglCreateContext(m_hdc); 
	wglMakeCurrent(m_hdc, m_glrc); 

	if (parent)
		resize(windowRect.right - windowRect.left,	windowRect.bottom - windowRect.top);
	else
		resize(640,	480);

	ShowWindow(hWnd, SW_SHOW); 

	Callbacks::setGifCallback(&m_renderer, GsRenderer::callback);

	initView();

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Display::makeCurrent()
{
	wglMakeCurrent(m_hdc, m_glrc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Display::resize(uint width, uint height)
{
	SetWindowPos(m_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREPOSITION | SWP_NOZORDER);
	makeCurrent();

	glViewport(0, 0, width, height);							

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();		

	//float aspect = (float)(width) / (float)(height);
	gluPerspective(90, 1.3333f, 1.0f, 200000.0);

	//glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Display::destroy()
{
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Display::setupDisplay()
{
	PIXELFORMATDESCRIPTOR *ppfd; 
	int pixelformat; 

	PIXELFORMATDESCRIPTOR pfd = 
	{ 
		sizeof(PIXELFORMATDESCRIPTOR), 
		1,                     
		PFD_DRAW_TO_WINDOW |   
		PFD_SUPPORT_OPENGL |   
		PFD_DOUBLEBUFFER,      
		PFD_TYPE_RGBA,         
		32,          
		0, 0, 0, 0, 
		0, 0, 0, 0,                 
		0, 
		0, 0, 0, 0,            
		0,            
		16,                     
		0,                     
		PFD_MAIN_PLANE,        
		0,                     
		0, 0, 0                
	}; 

	pfd.cColorBits = (BYTE)GetDeviceCaps(m_hdc,BITSPIXEL); 
	ppfd = &pfd; 

	pixelformat = ChoosePixelFormat(m_hdc, ppfd); 

	if ((pixelformat = ChoosePixelFormat(m_hdc, ppfd)) == 0) 
		return false; 

	if (!SetPixelFormat(m_hdc, pixelformat, ppfd)) 
		return false; 

	m_glrc = wglCreateContext(m_hdc);

	if (!m_glrc)
		return false;

	if (!wglMakeCurrent(m_hdc, m_glrc))
		return false;

	ShowWindow(m_hwnd, SW_SHOW);

	return true; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LONG WINAPI Display::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	LONG lRet = 1; 

	static int prevX = 0;
	static int prevY = 0;
	s_isMovingMouse = false;
	s_isMovingPos = false;

	switch (uMsg) 
	{ 
		case WM_CREATE: 
		{ 
			break;
		}

		case WM_SIZE: 
		{ 
			break;		
		}

		case WM_DESTROY: 
		{ 
			break;
		}

		case WM_MBUTTONDOWN :
		case WM_LBUTTONDOWN :
		{
			POINTS points = MAKEPOINTS(lParam);
			prevX = points.x;
			prevY = points.y;
			break;
		}

		case WM_MOUSEMOVE :
		{
			if (wParam == MK_MBUTTON)
				s_isMovingPos = true;

			if (wParam == MK_LBUTTON || wParam == MK_MBUTTON)
			{
				POINTS points = MAKEPOINTS(lParam);

				int deltaX = points.x - prevX;
				int deltaY = points.y - prevY;

				s_mouseDeltaX = deltaX * 0.1f;
				s_mouseDeltaY = deltaY * 0.1f;

				prevX = points.x;
				prevY = points.y;

				s_isMovingMouse = true;
			}

			break;
		}

		case WM_KEYDOWN: 
		{ 
			m_state = false;		// temporary
			PostQuitMessage(0);
			return 0;
		}

		default :
			return (LONG)DefWindowProc(hWnd, uMsg, wParam, lParam); 

		break; 
  } 

  return lRet; 
} 


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Display::update()
{
	MSG msg; 

	SwapBuffers(m_hdc);

	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) 
	{ 
		TranslateMessage(&msg); 
		DispatchMessage(&msg); 
	} 

	return m_state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Display::initView()
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0f, 1.0, 0.1, 10000000.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0f, 1.0f, 1.0f, 0.7f);
	glViewport(0, 0, m_width, m_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}


#if defined(_MSC_VER)
#pragma warning(default : 4312) // 'type cast' : conversion from 'LONG' to 'ps2emu::Display *' of greater size 
#pragma warning(default : 4311) // type cast' : pointer truncation from 'LPVOID' to 'LONG' 
#endif
