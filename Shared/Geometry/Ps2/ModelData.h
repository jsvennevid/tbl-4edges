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

#ifndef zenic_ps2_ModelData_h
#define zenic_ps2_ModelData_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../ModelData.h"
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>
#include <Shared/Graphics/Renderer/Ps2/Material.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace ps2
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class ModelData : public zenic::ModelData
{
	ZENIC_SERIALIZABLE_FACTORY;
public:

	enum ModelType
	{
		Normal,
		RefChained
	};

	enum ShaderPipeline
	{
		// Dynamic Pipeline(s)

		Dynamic,

		// Static Pipelines

		IntFlatNonShaded,
		IntTexturedNonShaded,

		FloatFlatNonShaded,
		FloatTexturedNonShaded,

		AlphaDissolveEnvmapped,
		Sprite
	};

	ModelData() {}

	void serialize(Serializer& s);

	void setPrimaryChain(const DataPtr<u8>& dataPtr);
	u8* primaryChain() const;

	ShaderPipeline shaderPipeline() const;
	void setShaderPipeline(ShaderPipeline shaderPipeline);

	void setType(ModelType type);

	void setMaterial(Material* material);
	const Material* material() const;

private:

	DataPtr<u8> m_primaryChain;
	DataPtr<u8> m_secondaryChain; // used if we need a doubled bufferd chain for some reason

	Material* m_material;

	u16 m_type;
	u16 m_shaderPipeline;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void	ModelData::setPrimaryChain(const DataPtr<u8>& dataPtr)
{
	m_primaryChain = dataPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ModelData::setType(ModelType type)
{
	m_type = u16(type);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u8* ModelData::primaryChain() const
{
	return m_primaryChain.objects();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ModelData::setMaterial(Material* material)
{
	m_material = material;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline const Material* ModelData::material() const
{
	return m_material;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline ModelData::ShaderPipeline ModelData::shaderPipeline() const
{
	return (ShaderPipeline)m_shaderPipeline;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void ModelData::setShaderPipeline(ModelData::ShaderPipeline shaderPipeline)
{
	m_shaderPipeline = (u16)shaderPipeline;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

#endif
