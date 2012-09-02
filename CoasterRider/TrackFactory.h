#pragma once

#include "Track.h"
#include "Orientation.h"
#include "irrlicht.h"

const Orientation default_track_orientation=Orientation(
							  vector3df(0.0,3.0,0.0)			//pos
							, vector3df(1.0,0.0,0.0), vector3df(0.0,1.0,0.0), vector3df(0.0,0.0,1.0)	//heading
							);

class TrackFactory
{
	protected:
	private:
		TrackFactory();
	public:
		virtual ~TrackFactory();
	public:	//creation functions:
		Track*createEmptyTrack(Orientation startori=default_track_orientation);
		Track*createTestTrack(Orientation startori=default_track_orientation);
		Track*createCorkscrewTrack(Orientation startori=default_track_orientation
		                           ,float trackscale=1.0f);
		Track*createMegaCoasterTrack(Orientation startori=default_track_orientation
		                             ,float trackscale=1.0f);
		Track*createTopHatCoasterTrack(Orientation startori=default_track_orientation
		                               ,float trackscale=1.0f);
		Track*createTrackFromFile(char*file);
	public: // other functions
		static TrackFactory*getinstance();
		void DefineElements();
		void SetTrackStart(Track*t,Orientation startori);
		inline void InitTables(Track*t)	{			t->initTablesFromPathSpline();	}
	private:	//elements:
		PathSpline
			 str8,lcorky,rcorky
			,lturn,rturn
			,hillup,hilldn
			,rqturn,lqturn
			,luhalfloop,ldhalfloop
			,ruhalfloop,rdhalfloop
			,valley30deg,crest30deg
			,valley90deg,crest90deg
			,valley45deg,crest45deg
			,lqroll,rqroll;
};


