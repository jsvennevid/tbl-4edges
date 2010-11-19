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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Graphics/Renderer/OpenGL/Renderer.h>
#include <Shared/Graphics/Renderer/DebugRenderer.h>
#include <gl/gl.h>
#include <gl/glu.h>


#include <Shared/Graphics/Scene/Model.h>
#include <Shared/Graphics/Scene/Camera.h>
#include <Shared/Geometry/OpenGL/ModelData.h>

#if defined(_MSC_VER)
#pragma warning(disable : 4312) // 'type cast' : conversion from 'LONG' to 'zenic::opengl::Renderer *' of greater size 
#pragma warning(disable : 4311) // type cast' : pointer truncation from 'LPVOID' to 'LONG' 
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

bool Renderer::m_state = false;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Renderer::Renderer() :
	m_renderingMode(Normal),
	m_camera(0)
{
	m_hdc = 0;
	m_hwnd = 0;
	m_glrc = 0;
	m_width = 1024;
	m_height = 768;
	m_fullScreen = false;
	m_state = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::setupDisplay()
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

bool Renderer::init(HINSTANCE instance, HWND parent, uint width, uint height, bool fullScreen)
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
	wndclass.lpfnWndProc   = (WNDPROC) WndProc; 
	wndclass.cbClsExtra    = 0; 
	wndclass.cbWndExtra    = 0; 
	wndclass.hInstance     = instance; 
	wndclass.hIcon         = 0;
	wndclass.hCursor       = 0; 
	wndclass.hbrBackground = 0; 
	wndclass.lpszMenuName  = "OpenGL Display"; 
	wndclass.lpszClassName = "OpenGL Display"; 

	if (!RegisterClass(&wndclass)) 
		return false; 

	HWND hWnd;

	if (parent)
	{
		GetClientRect(parent, &windowRect);
		AdjustWindowRectEx(&windowRect, WS_CHILD, FALSE, 0);	

		hWnd = CreateWindowEx(0, "OpenGL Display", "OpenGL Display", 
							  WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
							  0, 0, windowRect.right - windowRect.left,	windowRect.bottom - windowRect.top, 
							  parent, 0, instance, (LPVOID)this); 
	}
	else
	{
		hWnd = CreateWindow("OpenGL Display", "OpenGL Display", WS_OVERLAPPEDWINDOW, 0, 0, 
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

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LONG WINAPI Renderer::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{ 
	LONG lRet = 1; 

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

void Renderer::makeCurrent()
{
	wglMakeCurrent(m_hdc, m_glrc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::resize(uint width, uint height)
{
	SetWindowPos(m_hwnd, 0, 0, 0, width, height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREPOSITION | SWP_NOZORDER);
	makeCurrent();

	glViewport(0, 0, width, height);							

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();		

	//float aspect = (float)(width) / (float)(height);
	gluPerspective(90, 1.3333f, 1.0f, 2000.0);

	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);	
	glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModels(Model** models, uint modelCount)
{
	if (modelCount == 0)
		return;

	switch (m_renderingMode)
	{
		case Normal :
		{
			for (u32 i = 0; i < modelCount; ++i)
				renderModel(models[i]);	

			break;
		}

		case BoundingBox :
		{
			ZENIC_ASSERT_DESC(m_camera, "No Camera assigned to the renderer");

			DebugRenderer* debugRenderer = DebugRenderer::instance();
			const Matrix4& cameraMatrix = m_camera->transform().fastInverse();

			for (u32 i = 0; i < modelCount; ++i)
			{
				//const Bound& bound = models[i]->bound();
				Matrix4 worldTransform = models[i]->worldTransform() * cameraMatrix;
				debugRenderer->cube(worldTransform.position(), worldTransform, 4.0f);
			}

			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderModel(Model* model)
{
	switch (static_cast<ModelData*>(model->modelData())->type())
	{
		case ModelData::Rigid : renderRigid(model); break;
		case ModelData::Skinned : renderSkinned(model); break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderRigid(Model* model)
{
	ModelData* modelData = static_cast<ModelData*>(model->modelData());
	DataPtr<ModelData::PolygonList>& polygonLists = modelData->polygons();

	if (modelData->material())
		modelData->material()->execute();

	uint offset = 8;
	
	for (uint i = 0; i < polygonLists.count(); ++i)
	{
		ModelData::PolygonList& polygonList = polygonLists[i];
		f32* stream = polygonList.vertexStream().objects();

		uint size = polygonList.vertexStream().count();
		size /= offset;	

		glBegin(GL_TRIANGLE_STRIP);

		for (uint p = 0; p < size; ++p)
		{
			Vector3 pos(stream[0], stream[1], stream[2]);

			glTexCoord2f(stream[6], stream[7]); 
			//glColor3f(stream[3], stream[4], stream[5]);
			glColor3f(0.7f, 0.7f, 0.7f);
			glVertex3f(pos.x, pos.y, pos.z);

			stream += offset; 
		}

		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::renderSkinned(Model* /*model*/)
{





}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::begin()
{
	wglMakeCurrent(m_hdc, m_glrc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::end()
{
	SwapBuffers(m_hdc);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::flip()
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

void Renderer::initView()
{
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0f, 1.0, 0.1, 1000.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.6f, 0.9f, 0.4f, 0.7f);
	glViewport(0, 0, m_width, m_height);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#if defined(_MSC_VER)
#pragma warning(default : 4312) // 'type cast' : conversion from 'LONG' to 'zenic::opengl::Renderer *' of greater size 
#pragma warning(default : 4311) // type cast' : pointer truncation from 'LPVOID' to 'LONG' 
#endif
