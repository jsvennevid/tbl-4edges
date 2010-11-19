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