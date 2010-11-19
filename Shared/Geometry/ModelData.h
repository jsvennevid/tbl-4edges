#ifndef zenic_ModelData_h
#define zenic_ModelData_h

#include <Shared/Base/Serialize/Serializable.h>

namespace zenic
{
	class BoneMap;
}

namespace zenic
{

class ModelData : public Serializable
{
public:

	ModelData();
	virtual ~ModelData();

	virtual void serialize(Serializer& s);

	void setBoneMap(BoneMap* boneMap);
	BoneMap* boneMap() const;

private:

	BoneMap* m_boneMap;
};

#include "ModelData.inl"

}

#endif
