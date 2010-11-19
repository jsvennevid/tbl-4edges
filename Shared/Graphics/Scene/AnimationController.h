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

#ifndef zenic_AnimationController_h
#define zenic_AnimationController_h

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Controller.h"
#include <Shared/Base/Serialize/SerializableFactory.h>
#include <Shared/Base/Serialize/SerializableStructure.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace zenic
{

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class AnimationController : public Controller
{
	ZENIC_SERIALIZABLE_FACTORY;

public:

	AnimationController();

	// Hardcoded tracks

	enum AnimTracks
	{
		PosX,
		PosY,
		PosZ,

		RotateX,
		RotateY,
		RotateZ,

		ScaleX,
		ScaleY,
		ScaleZ
	};


	// TODO: Split this into separate tracks.

	struct Track
	{
		ZENIC_SERIALIZABLE_STRUCTURE;

		enum
		{
			Uncompressed,
			Compressed16bit
		};

		inline Track()
		{
			cachedIndex = 0;
			type = 0;
			compression = Compressed16bit;
		}

		DataPtr<u8> data;
		u16 cachedIndex;
		u8 type;
		u8 compression;
	};

	void update(float time, Matrix4& matrix);
	void serialize(Serializer& s);

	void setTrackCount(u32 streams);
	u32 trackCount() const;

	void setTrack(u32 index, Track& stream);
	Track& track(u32 index) const;

private:

	struct Key
	{
		float time;
		float value;
	};

	struct Key16bit
	{
		u16 time;
		u16 value;
	};

	float evaluate(Track& track, float time);
	float evaluateUncompressed(Track& track, float time);
	float evaluateCompressed16bit(Track& track, float time);

	DataPtr<Track> m_tracks;
	float m_time;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AnimationController::AnimationController() : m_time(0.0f)
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void AnimationController::setTrackCount(u32 streams)
{
	m_tracks.resize(streams);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline u32 AnimationController::trackCount() const
{
	return m_tracks.count();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline void AnimationController::setTrack(u32 index, Track& stream)
{
	ZENIC_ASSERT(index < m_tracks.count());
	m_tracks[index] = stream;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline AnimationController::Track& AnimationController::track(u32 index) const
{
	ZENIC_ASSERT(index < m_tracks.count());
	return m_tracks[index];
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif

