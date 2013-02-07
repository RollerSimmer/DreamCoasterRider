#include "TrackLabeler.h"
#include <cstring>
#include <string>

using namespace std;

/**#################################################################
	TrackLabeler() - construct a TrackLabeler object
##################################################################*/

TrackLabeler::TrackLabeler(ISceneManager*_smgr
		             ,IGUIEnvironment*_guienv
		             ,TrackOperator*_trackop 	)
		:	smgr(_smgr)
		,	trackop(_trackop)
		,	guienv(_guienv)
	{
	//ctor
	}

/**#################################################################
	TrackLabeler() - destruct a TrackLabeler object
##################################################################*/

TrackLabeler::~TrackLabeler()
	{
	//dtor
	}

/**#################################################################
	LabelTrack() - add labels to parts of track based on block types.
##################################################################*/

void TrackLabeler::LabelTrack()
	{
	Track*track=trackop->track;
	Orientation ori,ori_board;
	float trackpos;

	DeleteLabels();

	IGUISkin*skin=guienv->getSkin();
	IGUIFont*font=skin->getFont();

	for(trackpos=0.0;trackpos<track->getTrackLen();trackpos+=5.0f)
		{
		//add a label to the scene and get its pointer:
			IBillboardTextSceneNode*labelnode;
			IBillboardSceneNode*boardnode;

			labelnode=smgr->addBillboardTextSceneNode
										( font
										 ,getLabelText(trackpos) );
			boardnode=smgr->addBillboardSceneNode();

		//move the position 3m off the track:
			const float labelht=2.75;
			ori=track->getbankedori(trackpos);
			ori.pos=ori.pos+ori.hdg.getup()*labelht;
			ori_board=ori;
			ori_board.pos=ori.pos+ori.hdg.getfwd()*0.01;
			if(labelnode)
				{
				labelnode->setPosition(ori.pos);
				labelnode->updateAbsolutePosition();
				}
			if(boardnode)
				{
				boardnode->setPosition(ori_board.pos);
				boardnode->updateAbsolutePosition();
				}

		//set color and size:
			if(labelnode)
				{
				labelnode->setColor(SColor(255,255,255,0));
				labelnode->setSize(dimension2df(1.5,0.2));
				}
			if(boardnode)
				{
				SColor boardcolor=SColor(255,0,0,127);
				boardcolor.setAlpha(255);
				boardnode->setColor(boardcolor);
				boardnode->setSize(dimension2df(1.6,0.2));
				}

		//add that pointer to label list:
			labels.push_back(labelnode);
			boards.push_back(boardnode);
		}
	}


/**#################################################################
	DeleteLabels() - delete all labels
##################################################################*/

void TrackLabeler::DeleteLabels()
	{
	while(!labels.empty())
		{
		smgr->addToDeletionQueue(labels.back());
		////labels.back()->drop();
		////delete labels.back();
		labels.pop_back();
		}
	labels.clear();		//clear the label list for good measure
	while(!boards.empty())
		{
		smgr->addToDeletionQueue(boards.back());
		boards.pop_back();
		}
	boards.clear();
	}

/**#################################################################
	getLabelText() - get the text for the label at a track position
		In:
			trackpos - the track position at which to find the appropriate label
		Out:
			(return value) - the label string in wide-character format.
##################################################################*/

const wchar_t*TrackLabeler::getLabelText(float trackpos)
	{
	static basic_string<wchar_t> s;		//the label string
	wchar_t ns[8];									//number string
	s.clear();
	Block::Type blocktype=trackop->getBlockType(trackpos);
	float trackht=getTrackHt(trackpos);
	switch(blocktype)
		{
		case Block::bt_trim: 	{ s=L"trim block";		break; }
		case Block::bt_lift: 	{ s=L"lift block";		break; }
		case Block::bt_boost:
			{ s=L"boost block";		break; }
		case Block::bt_station:
			{ s=L"station block";	break; }
		case Block::bt_normal:
		default:						{ s=L"normal block";		break; }
		}
	//add position to the string:
		s+=L"(";
		swprintf(
		ns
		#ifndef DEBUG_WINDOWS
		,sizeof(ns)/sizeof(ns[0])-1
		#endif
		,L"%0.1f"
		,trackpos);
		s+=ns;
		s+=L"m, ht=";
		swprintf
		(
		ns
		#ifndef DEBUG_WINDOWS
		,sizeof(ns)/sizeof(ns[0])-1
		#endif
		,L"%0.1f",trackht);
		s+=ns;
		s+=L"m)";
	return s.c_str();
	}


/**##################################################################
	getTrackHt() - gets the track height above ground/base height.
		In: trackpos - the track position
		Out: (return value) - the track height in meters.
###################################################################*/

float TrackLabeler::getTrackHt(float trackpos)
	{
	return trackop->track->htAt(trackpos);
	}

/**##################################################################
	getBlockType() - toggle the visibility of each individual label
###################################################################*/
void TrackLabeler::ToggleVisibility()
	{

	for(	deque<IBillboardTextSceneNode*>::iterator it=labels.begin()
			;it!=labels.end();it++)
		{
		(*it)->setVisible(!(*it)->isVisible());
		}
	for(	deque<IBillboardSceneNode*>::iterator it=boards.begin()
			;it!=boards.end();it++)
		{
		(*it)->setVisible(!(*it)->isVisible());
		}
	}
