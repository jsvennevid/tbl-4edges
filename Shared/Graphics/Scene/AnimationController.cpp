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

#include "AnimationController.h"

#include <Shared/Base/Io/Log.h>
#include <Shared/Base/Math/Matrix4.h>
#include <Shared/Base/Serialize/Serializer.h>
#include <Shared/Base/Serialize/SerializableVersion.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

ZENIC_CREATE_SERIALIZABLE_FACTORY(AnimationController, SERIALIZABLE_ID('C','N','T','R'), SERIALIZABLE_ID('A','N','I','M'));
ZENIC_CREATE_SERIALIZABLE_STRUCTURE(AnimationController, Track, SERIALIZABLE_ID('T','R','A','K'));

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationController::serialize(Serializer& s)
{
	zenic::SerializableVersion version(1, factory());

	s << version;

	s.beginStructure(m_tracks, SERIALIZER_NAME("m_tracks"));

	for (uint i = 0, n = m_tracks.count(); i < n; ++i)
		m_tracks[i].serialize(s);

	s.endStructure();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationController::Track::serialize(Serializer& s)
{
	s.descriptor(SERIALIZER_NAME("data")) << data;
	s.descriptor(SERIALIZER_NAME("type")) << type;
	s.descriptor(SERIALIZER_NAME("compression")) << type;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AnimationController::update(float time, Matrix4& matrix)
{
	// TODO: Use a global time instead of incresing here all the time?

	float posX = evaluate(m_tracks[PosX], time);
	float posY = evaluate(m_tracks[PosY], time);
	float posZ = evaluate(m_tracks[PosZ], time);
	//ZENIC_INFO(posX << " " << posY << " " << posZ);

	float rotateX = evaluate(m_tracks[RotateX], time);
	float rotateY = evaluate(m_tracks[RotateY], time);
	float rotateZ = evaluate(m_tracks[RotateZ], time);

	Matrix4 rotY;
	Matrix4 rotX;
	Matrix4 rotZ;

	rotX.makeXrotation(rotateX);
	rotY.makeYrotation(rotateY);
	rotZ.makeZrotation(rotateZ);

	matrix = rotX * rotY * rotZ;
	matrix.setPosition(Vector3(posX, posY, posZ));
}

float AnimationController::evaluate(Track& track, float time)
{
	switch (track.compression)
	{
		case Track::Uncompressed : return evaluateUncompressed(track, time);
		case Track::Compressed16bit : return evaluateCompressed16bit(track, time);
	}

	return 0.0f;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float AnimationController::evaluateUncompressed(Track& track, float time)
{
	uint keyCount = track.data.count() / 2;
	const Key* keys = reinterpret_cast<const Key*>(track.data.objects());

	if (keyCount == 0)
		return 0.0f;

	if (keyCount == 1)
		return keys[0].value;

	// check if at the very start

	if (time < keys[0].time)
		return keys[0].value;

	// Check if we are at the end

	if (time >= keys[keyCount-1].time)
		return keys[keyCount-1].value;

	uint channelIt = 0;

	//get the endpoints of the interval being evaluated

	while (time > keys[channelIt+1].time)
		++channelIt;

	const Key* key0 = &keys[channelIt];
	const Key* key1 = &keys[channelIt+1];

	//get interval length, time in [0, 1]

	float t = (time - key0->time) / (key1->time - key0->time);
	return key0->value + t * (key1->value - key0->value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline float AnimationController::evaluateCompressed16bit(Track& track, float time)
{
	uint keyCount = (track.data.count() - 16) / 4;
	const float* floatData = reinterpret_cast<const float*>(track.data.objects());
	const Key16bit* keys = reinterpret_cast<Key16bit*>(track.data.objects() + 16);

	//ZENIC_ASSERT(floatData);
	//ZENIC_ASSERT(keys);

	if (!floatData)
		return 0.0f;

	if (keyCount == 0)
		return 0.0f;

	float scaleValue = floatData[0];
	float offsetValue = floatData[1];

	if (keyCount == 1)
		return (keys[0].value * scaleValue) + offsetValue;

	float scaleTime = floatData[2];
	float offsetTime = floatData[3];

	// check if at the very start

	if (time < ((keys[0].time * scaleTime) + offsetTime))
		return (keys[0].value * scaleValue) + offsetValue;

	// Check if we are at the end

	if (time >= ((keys[keyCount-1].time * scaleTime) + offsetTime))
		return (keys[keyCount-1].value * scaleValue) + offsetValue;

	//get the endpoints of the interval being evaluated

	uint index = 0;

	while (time > ((keys[index+1].time * scaleTime) + offsetTime))
		++index;

	const Key16bit* key16bit0 = &keys[index];
	const Key16bit* key16bit1 = &keys[index+1];

	// "Unpack" the keys, can be made faster, but this should be on vu0 on ps2 anyway.

	Key key0;
	Key key1;

	key0.time = (key16bit0->time * scaleTime) + offsetTime;
	key0.value = (key16bit0->value * scaleValue) + offsetValue;

	key1.time = (key16bit1->time * scaleTime) + offsetTime;
	key1.value = (key16bit1->value * scaleValue) + offsetValue;

	//get interval length, time in [0, 1]

	float t = (time - key0.time) / (key1.time - key0.time);
	return key0.value + t * (key1.value - key0.value);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

