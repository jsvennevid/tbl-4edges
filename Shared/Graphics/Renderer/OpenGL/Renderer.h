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

#ifndef zenic_opengl_Renderer_h
#define zenic_opengl_Renderer_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <Shared/Base/Types.h>
#include <Shared/Base/Debug/Assert.h>
#include <Shared/Base/Storage/Singleton.h>
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class Model;
	class Camera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Renderer
{
public:
	
	enum Mode
	{
		Normal,
		BoundingBox,
	};

	Renderer();

	bool init(HINSTANCE instance, HWND parent, uint width, uint height, bool fullScreen = false);
	bool flip();

	void renderModel(Model* model);
	void renderModels(Model** models, uint modelCount);

	bool isRunning() const;
	void makeCurrent();
	void resize(uint width, uint height);

	void begin();
	void end();

	Mode renderingMode() const;
	void setRenderingMode(Mode renderingMode);

	u32 fullScreen() const;
	void setFullScreen(u32 fullScreen);

	Camera* camera() const;
	void setCamera(Camera* camera);

private:

	static LONG WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); 
	bool setupDisplay();
	void initView();

	void renderRigid(Model* model);
	void renderSkinned(Model* model);

	Camera* m_camera;

	HDC m_hdc;
	HWND m_hwnd;
	HGLRC m_glrc;

	uint m_width;
	uint m_height;
	
	Mode m_renderingMode;

	u32 m_fullScreen:1;

	static bool m_state;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline bool Renderer::isRunning() const
{
	return m_state;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 Renderer::fullScreen() const
{
	return m_fullScreen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setFullScreen(u32 fullScreen)
{
	m_fullScreen = fullScreen;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Renderer::Mode Renderer::renderingMode() const
{
	return m_renderingMode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setRenderingMode(Renderer::Mode renderingMode)
{
	m_renderingMode = renderingMode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline Camera* Renderer::camera() const
{
	return m_camera;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void Renderer::setCamera(Camera* camera)
{
	m_camera = camera;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //zenic_opengl_Renderer_h
