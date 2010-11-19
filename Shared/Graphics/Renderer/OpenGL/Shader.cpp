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
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement and may not be copied or disclosed except in
// accordance with the terms of that agreement.
//
// Copyright (c) 2005 Jesper Svennevid, Daniel Collin.
// All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Shader.h"
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{
	namespace opengl
	{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ZENIC_CREATE_SERIALIZABLE_FACTORY(Shader, SERIALIZABLE_ID('O','G','L',' '), SERIALIZABLE_ID('S','H','D','R'))

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Shader::Shader()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shader::serialize(Serializer& s)
{
	SerializableVersion version(1, factory());

	s << version;

	s.descriptor(SERIALIZER_NAME("m_shaderNodes")) << m_shaderNodes;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shader::execute()
{
	for (uint i = 0; i < m_shaderNodes.count(); ++i)
		m_shaderNodes[i]->execute();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Shader::addInternal(Serializer& s)
{
	for (uint i = 0; i < m_shaderNodes.count(); ++i)
		s.add(m_shaderNodes[i]);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}

