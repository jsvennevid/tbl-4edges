#include "ModelData.h"

#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>
#include <Shared/Graphics/Scene/BoneMap.h>

namespace zenic
{

ModelData::~ModelData()
{
}

void ModelData::serialize(Serializer& s)
{
	SerializableVersion version(1);

	s << version;

	s.descriptor("m_boneMap") << m_boneMap;
}

}
