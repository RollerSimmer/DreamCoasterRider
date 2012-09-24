#pragma once

#include "irrlicht.h"
#include "TrackPatterns/TrackMeshPattern.h"
#include "SupportMesh.h"
#include "TrackVertex.h"
#include <vector>

class Track;

class TrackMesh
	{
	private:
		f32 seglen;					//the length of the rail bars, or segments.
		f32 runglen;				//the length of the crossbars (or track width)
		f32 scale;					//the overall scale of the mesh.
		Track*track;				//the track, which defines path and headings.
		int amtsegs;				//the amount of segments. (instances of pattern)
		TrackMeshPattern*pat;	//the pattern to duplicate along the track
		int segsPerSupport;		//support spacing
		int firstsupidx;			//index of first support mesh buffer
		int amttrackverts;		//amount of track vertices
		int amtsupverts;			//amount of support vertices
		int amtchainverts;		//amount of chain vertices
		int amtcatwalkverts;		//amount of catwalk vertices
		int amtstationverts;		//amount of station vertices		
		vector<TrackVertex> vertices;		//track, support, station, and accessory vertices		
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

