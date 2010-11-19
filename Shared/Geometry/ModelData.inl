inline ModelData::ModelData() : m_boneMap(0)
{
}

inline void ModelData::setBoneMap(BoneMap* boneMap)
{
	m_boneMap = boneMap;
}

inline BoneMap* ModelData::boneMap() const
{
	return m_boneMap;
}
