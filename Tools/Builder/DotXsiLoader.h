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
#ifndef zenic_DotXsiLoader_h
#define zenic_DotXsiLoader_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Types.h>
#include <vector>
#include <map>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CSLScene;
class CSLModel;
class CSLXSIMaterial;

namespace zenic
{
	class ExporterBackend;
	class OutSerializer;	// Should be OutSerializer interface
	class Serializable;
	class Node;
	class Material;
	class SceneInfo;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
class DotXsiLoader
{
public:
	
	DotXsiLoader();
	~DotXsiLoader();

	bool load(const char* filename);
	void process(ExporterBackend& backend, OutSerializer& serializer, bool oneObject);

private:

	static void warningCallBack(char* string, int value);

	void addModel(CSLModel* model);

	Node* traverseRecursive(CSLModel* child, ExporterBackend& backend, SceneInfo& sceneInfo, int level, bool oneObject);

	void buildMaterials(ExporterBackend& backend);
	void buildImages(ExporterBackend& backend);
	void saveMaterialAndImages();

	OutSerializer* m_serializer;
	CSLScene* m_scene;

	std::vector<CSLModel*> m_meshList;
	std::vector<Serializable*> m_textures;
	std::vector<Material*> m_materials;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void DotXsiLoader::addModel(CSLModel* model)
{
	m_meshList.push_back(model);	
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


}

#endif
