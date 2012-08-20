#pragma once

#include "irrlicht.h"

class TrackMeshPattern : public SMeshBuffer
	{
	public: // public data
		#if 0
		int supportSnapVertexIdx;	/* the index of 
												the invisible vertex at which 
		                           	the support structure snaps
											   to the track: */
		int supportSideVertexIdx;	/* the index of 
												the invisible vertex at which 
												the support arm for upside-down
												track starts going downward. */
		float supportSnapTrackPos;	/* the progression of the support along 
												the track segment */
		#endif 
	public:
		TrackMeshPattern();
	protected:
	};


