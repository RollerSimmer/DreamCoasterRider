#pragma once

#include "../irrlicht.h"
#include "TrackVertex.h"
#include "../Track.h"
#include "TrackColors.h"

//define some classes for friends:
	class TrackPartPatternFactory;
	class RailPatternFactory;
	class RungPatternFactory;
	class TrackPartMesh;


/**#########################################################################
	TrackPartPattern - a class for subdividing track into parts.
		A track will be made up of many patterns repeated over the duration
		of the track.  The composite of all these patterns will make a single
		part of the track, like a rail, crossbar or handrail.  All of these
		parts make up the entire track.
##########################################################################*/

class TrackPartPattern
	{
	public:
		friend class TrackPartPatternFactory;
		friend class RailPatternFactory;
		friend class TrackPartMesh;
		friend class RungPatternFactory;
	protected:	//state
		TrackColors&colors;								//the color of the element.
		bool visible;								//should this element be shown?
		deque<TrackVertex> vertices;			//the vertices of the part pattern.
		deque<unsigned short> indices;		//the vertex indices, grouped in triples, which define
		                                    //triangles and shapes.
		Track*track;								//a pointer to the track for various purposes.
	public:	//constructor
		TrackPartPattern(TrackColors&_colors);

	public: //functions
		IMeshBuffer*MakeMeshBuffer();
		void addK(float ko);
		static void FixNormals(SMeshBuffer*buf);
	};


