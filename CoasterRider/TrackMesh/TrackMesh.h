#pragma once

#include "irrlicht.h"
#include "TrackVertex.h"
#include "TrackPartMesh.h"
#include <deque>

using namespace std;

class Track;

/**###################################################################################
	TrackMesh - The mesh for a track. It defines the Irrlicht mesh geometry while
	            keeping some logic about what track part is what.  It needs to refer
	            to a track and a track operator constantly for the whereabouts of
	            track and its blocks.  It is primarily for making meshes that Irrlicht
	            can understand, while preserving track geometry and integrity.
	            Editing a track will cause a TrackMesh to have to be updated, and
	            in turn the IMesh to also be updated as well.
####################################################################################*/

class TrackMesh
	{
	public: //type
		enum	PatternType	// track pattern types
			{
			pat_ladder=0				//simple ladder track found on many junior coasters
			,pat_corkscrew				//steel corkscrew like Arrow/Vekoma
			,pat_looper					//steel looper like Schwarzkopf
			,pat_beamer					//steel twister like B&M
			,pat_lattice				//steel twister/giga like Intamin
			,pat_wood					//good ol' woody track.
			};
	protected:	//state
		TrackPartMesh*cat;				//the catwalk
		TrackPartMesh*handrail;			//the handrail
		TrackPartMesh*chain;				//the lift chain
		TrackPartMesh*rungs;				//the "rungs", or track crossbars
		TrackPartMesh*rails;				//the track rails
		TrackPartMesh*station;			//the station platform
		TrackPartMesh*supports;			//the supports
		Track*track;						//the track that dictates the shape of the mesh
		TrackOperator*trackop;			//the track operator, for determining blocks,
												//		and which parts are visible/added for each block.
		deque<TrackVertex> supSnaps;	//support snap points (usually the bottom of the track
		deque<TrackVertex> supSides;	//support side points - where the "side" of the track
		                              //		is for inverted "pull" supports
		                              //		(normal supports "push" up from the ground,
		                              //		inverted ones "pull" the track)
		float trackwidth;					//the width of the track, usually rung length across.
		float seglen;						//the segment length.  The length of a single pattern
												//		in terms of track progression
		float scale;						//the scale of the track.  for scaling whole tracks...
												//		(mini track or jumbo track perhaps?)
		int amtRungsPerSupport;			//the amount of rungs to skip before building a support
												//		(supports may be skipped if there is something in
												//     the way)
		TrackColors colors;				//the track colors
	public:	//state
		IMesh*mesh;							//the mesh for Irrlicht's environment
		IMeshSceneNode*node;				//the Irrlicht mesh node for this mesh.
		IMeshManipulator*manip;			//the Irrlicht mesh manipulator.
		ISceneManager*smgr;				//the Irrlicht scene manager
		IrrlichtDevice*device;			//the Irrlicht device.
		IVideoDriver*driver;				//The Irrlicht video driver.
	public: //constructor
		TrackMesh(Track*_track,TrackOperator*_trackop
		          ,ISceneManager*_smgr,IrrlichtDevice*_device
		          ,IVideoDriver*_driver
		          ,SColor _color1    ,SColor _color2
		          ,SColor _color3    ,SColor _color4
		          ,SColor _specular  ,float shininess
		          );
	public: //function
		void MakeCatwalkPlatforms(PatternType type);
		void MakeCatwalkHandrails(PatternType type);
		void MakeChainlifts(PatternType type);
		void MakeSupports(PatternType type);
		void MakeTrackRungs(PatternType type);
		void MakeTrackRails(PatternType type);
		void MakeTrack(PatternType type,bool doBuildMesh);
		void BuildMesh();
		void DestroyMesh();
		void RebuildMesh();
		void ConformToTrack();
		void FixNormals();
	};

