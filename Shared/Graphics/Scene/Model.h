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

#ifndef zenic_Model_h
#define zenic_Model_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Node.h"
#include <Shared/Base/Storage/Array.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	class ModelData;
	class BoneArray;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Model : public Node
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	Model();
	virtual ~Model();

	virtual void update(float time);
	virtual void serialize(Serializer& s);

	static void resetCollectedModelCount();

	ModelData* modelData() const;
	void setModelData(ModelData* modelData);

	BoneArray* boneArray() const;
	void setBoneArray(BoneArray* boneArray);

	Model* next() const;
	void setNext(Model* next);

	static Model** collectedModels();
	static uint collectedModelsCount();

private:

	ModelData* m_modelData;
	BoneArray* m_boneArray;
	Model* m_next;

	static Model* m_collectedModels[300];
	static Model** m_collectedModelsList;
	static uint m_collectedModelsCount;
};

#include "Model.inl"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
