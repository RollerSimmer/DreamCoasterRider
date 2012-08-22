#pragma once

#include "Track.h"
#include "irrlicht.h"

class TrackFactory
{
	protected:
	private:
		TrackFactory();
	public:
		virtual ~TrackFactory();
	public:
		static TrackFactory*getinstance();
		Track*createTestTrack(vector3df startpos=vector3df(0.0,0.5,0.0));
};


