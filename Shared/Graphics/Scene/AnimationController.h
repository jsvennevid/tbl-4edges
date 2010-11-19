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

