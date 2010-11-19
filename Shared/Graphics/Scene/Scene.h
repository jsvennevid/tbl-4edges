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

#ifndef zenic_Scene_h
#define zenic_Scene_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Shared/Base/Serialize/SerializableFactory.h>

#include "AnimationController.h"
#include "BoneArray.h"
#include "BoneMap.h"
#include "Model.h"
#include "Node.h"
#include "Camera.h"
#include "SceneInfo.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

ZENIC_REFERENCE_SERIALIZABLE_FACTORY(BoneArray);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(BoneMap);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(Model);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(Node);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(Camera);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(SceneInfo);
ZENIC_REFERENCE_SERIALIZABLE_FACTORY(AnimationController);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
