#pragma once

#include <irrlicht.h>
using namespace irr;

class Track;

class TrackNode : public scene::ISceneNode
	{
	protected:
		Track*track;
		core::aabbox3d<f32> Box;
		video::S3DVertex Vertices[36];
		s16 nverts;
		video::SMaterial Material;
	public:
		TrackNode(
			 ISceneNode*parent,scene::ISceneManager*mgr,s32 id
			,const core::vector3df &  	position = core::vector3df(0,0,0)
			,const core::vector3df &  	rotation = core::vector3df(0,0,0)
			,const core::vector3df &  	scale = core::vector3df(1,1,1)
			,Track*_track=0
			);
	public:
		virtual void OnRegisterSceneNode();
		virtual void render();
		virtual const core::aabbox3d<f32>& getBoundingBox() const;
		virtual u32 getMaterialCount() const	{	return 1;	}
	};
