#ifndef TRACKLABELER_H
#define TRACKLABELER_H

#include "../irrlicht.h"
#include "../TrackOperator.h"

#include <deque>

class TrackLabeler
	{
	public: //data
		ISceneManager*smgr;
		IGUIEnvironment*guienv;
		TrackOperator*trackop;
		deque<IBillboardTextSceneNode*> labels;
		deque<IBillboardSceneNode*> boards;
	public:	//tor
		TrackLabeler(ISceneManager*_smgr=0
		             ,IGUIEnvironment*_guienv=0
		             ,TrackOperator*_trackop=0 	);
		virtual ~TrackLabeler();
	public: //functions
		void LabelTrack();
		void DeleteLabels();
		void ToggleVisibility();
	protected: //private functions for getting label text
		const wchar_t*getLabelText(float trackpos);
		Block::Type getBlockType(float trackpos)	;
	};

#endif // TRACKLABELER_H
