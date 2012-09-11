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
		ISceneManager*smgr;
		IVideoDriver*driver;
	public:
		SMesh*smesh;
		IMeshSceneNode* meshnode;
		IMesh*imesh;
	public:	//ctor & dtor
		TrackMesh( Track*_track,ISceneManager*_smgr,IVideoDriver*_driver )
				: imesh(0), smesh(0),meshnode(0),scale(0.f), track(_track)
				, smgr(_smgr), driver(_driver)
			{			smesh=new SMesh();	amtsegs=amtsupports=firstsupidx=0;			}
		~TrackMesh()			{			/*imesh->drop();*/	smesh->drop();			}
	public:	//accessor
		void settrack(Track*_track)	{	track=_track;	}
	public:	//functions
		void CleanUpForInit();
		void init(Track*t,f32 _scale);
		TrackMeshPattern*MakeSegFromPattern(TrackMeshPattern*pat,int pos);
		void ConformMeshToTrackSpline();
		void FixNormals();
		void AddSupports();
		void DelSupports();
		void UpdateSupports();
		void RecalculateAllBoundingBoxes();
		void AddToScene();
		void SetNodeMeshToSimpleMesh();
	};

