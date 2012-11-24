#pragma once

#include "../../irrlicht.h"
#include "TrackPartPatternFactory.h"
#include <deque>

class TrackMesh;

class SupportPatternFactory: public TrackPartPatternFactory
{
	private: //state
		TrackPartPattern*pat;
		////SColor color;
	private: //constructor:
		SupportPatternFactory(): TrackPartPatternFactory()	{}
	public:	//destructor:
		~SupportPatternFactory()	{}
	public:	//functions:
		static SupportPatternFactory*getinstance();
		virtual TrackPartPattern*create( int type
												  ,int segi
												  ,f32 patlen
		                                ,TrackMesh*tmesh		                                
		                                ////,SpaceMap*spacemap
		                                ,TrackColors&colors=*((TrackColors*)0)
		                                );		
		void addTube(float k,vector3df p1,vector3df p2,float r,SColor color);
		void addBeam( float k,vector3df p1,vector3df p2,float w,float h
		            /*,Orientation*rt,Orientation*up*/
		            ,SColor color);
		void addFrameBoxTower_noCollision
								(
								float k
								,vector3df lf,vector3df rf
								,vector3df lb,vector3df rb
								,float dim
								,SColor color
								,bool placeBackBar
								,bool placediag
								);		            
	private:	//internal methods:
};
