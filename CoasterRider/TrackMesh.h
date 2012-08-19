#pragma once

#include "irrlicht.h"
#include <vector>

class Track;

class TrackMesh
	{
	private:
		f32 seglen;
		f32 runglen;
		f32 scale;
		Track*track;
		int amtsegs;
		std::vector<SMeshBuffer*>segary;
	public:
		SMesh*mesh;
	public:	//ctor & dtor
		TrackMesh(Track*_track) : mesh(0),scale(0.f), track(_track)
			{			mesh=new SMesh();			}
		~TrackMesh()			{			mesh->drop();			}
	public:	//accessor
		void settrack(Track*_track)	{	track=_track;	}
	public:	//functions
		void CleanUpForInit();
		void init(Track*t,f32 _scale,IVideoDriver*driver);
		SMeshBuffer*MakeSegFromPattern(SMeshBuffer*pat,int pos);
		SMeshBuffer*CreateLadderPattern( f32 raillen,f32 rungwidth,f32 railrad,f32 rungRad
		                                ,SColor patcolor1,SColor patcolor2,IVideoDriver*driver);
		void ConformMeshToTrackSpline();
		void FixNormals();
	};

