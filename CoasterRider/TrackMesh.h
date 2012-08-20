#pragma once

#include "irrlicht.h"
#include "TrackMeshPattern.h"
#include "SupportMesh.h"
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
		std::vector<TrackMeshPattern*>segary;
		int segsPerSupport;		//support spacing
		int firstsupidx;			//index of first support mesh buffer
		int amtsupports;			//amount of support mesh buffers
		std::vector<SupportMesh*>supary;
	public:
		ISceneCollisionManager*colmgr;
		IVideoDriver*driver;
	public:	
		SMesh*mesh;
	public:	//ctor & dtor
		TrackMesh( Track*_track,ISceneCollisionManager*_colmgr,IVideoDriver*_driver ) 
				: mesh(0),scale(0.f), track(_track)
				, colmgr(_colmgr), driver(_driver)
			{			mesh=new SMesh();	amtsegs=amtsupports=firstsupidx=0;			}
		~TrackMesh()			{			mesh->drop();			}
	public:	//accessor
		void settrack(Track*_track)	{	track=_track;	}
	public:	//functions
		void CleanUpForInit();
		void init(Track*t,f32 _scale,IVideoDriver*driver);
		TrackMeshPattern*MakeSegFromPattern(TrackMeshPattern*pat,int pos);
		void ConformMeshToTrackSpline();
		void FixNormals();
		void AddSupports();
		void DelSupports();
		void UpdateSupports();
		void RecalculateAllBoundingBoxes();
	};

