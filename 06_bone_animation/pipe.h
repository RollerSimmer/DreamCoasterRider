#pragma once

#include <vector>
#include <iostream>

class PipeNode: public scene::ISceneNode
{

	/*
	First, we declare some member variables:
	The bounding box, 4 vertices, and the material of the tetraeder.
	*/
	core::aabbox3d<f32> Box;
	std::vector<video::S3DVertex> vertices;
	video::SMaterial Material;

	/*
	The parameters of the constructor specify the parent of the scene node,
	a pointer to the scene manager, and an id of the scene node.
	In the constructor we call the parent class' constructor,
	set some properties of the material, and
	create the 4 vertices of the tetraeder we will draw later.
	*/

public:
	float radius;
	short ndivs;
	float height;
	bool doInner;
	bool doOuter;

	~PipeNode()
		{
		}
	PipeNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id
				,float sradius,short sndivs, video::SColor color,float sheight
				,bool sdoInner,bool sdoOuter)
		: scene::ISceneNode(parent, mgr, id)
		{
		radius=sradius;
		ndivs=sndivs;
		height=sheight;
		doInner=sdoInner;
		doOuter=sdoOuter;

		Material.Wireframe = false;
		Material.Lighting = false;

		const float dpi=core::PI*2;	//double pi
		float angradstep=dpi/(float)ndivs;
		float angrad=0.0;

		short i;

		//alloc vertex space
		while(vertices.size()>0)
			vertices.pop_back();
		for(short i=0;i<ndivs*2+2;i++)
			vertices.push_back(video::S3DVertex());

		//make two rings of points
		video::SColor dynamicColor;
		for(i=0,angrad=0.0;i<ndivs+1;i++,angrad+=angradstep)
			{
			////float lightness=(float)i*255.0/(float)ndivs;
			////dynamicColor=video::SColor(255,lightness,lightness,lightness);
			vertices[i]
					= video::S3DVertex(radius*cos(angrad),+height/2,radius*sin(angrad), 1,1,0,
							color, 0, 1);
			vertices[ndivs+i+1]
					= video::S3DVertex(radius*cos(angrad),-height/2,radius*sin(angrad), 1,1,0,
							color, 0, 1);
			}

		/*
		The Irrlicht Engine needs to know the bounding box of a scene node.
		It will use it for automatic culling and other things. Hence, we
		need to create a bounding box from the 4 vertices we use.
		If you do not want the engine to use the box for automatic culling,
		and/or don't want to create the box, you could also call
		irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
		*/
		Box.reset(vertices[0].Pos);
		for (s32 i=0; i<ndivs*2; ++i)
			Box.addInternalPoint(vertices[i].Pos);
		}

	/*
	Before it is drawn, the irr::scene::ISceneNode::OnRegisterSceneNode()
	method of every scene node in the scene is called by the scene manager.
	If the scene node wishes to draw itself, it may register itself in the
	scene manager to be drawn. This is necessary to tell the scene manager
	when it should call irr::scene::ISceneNode::render(). For
	example, normal scene nodes render their content one after another,
	while stencil buffer shadows would like to be drawn after all other
	scene nodes. And camera or light scene nodes need to be rendered before
	all other scene nodes (if at all). So here we simply register the
	scene node to render normally. If we would like to let it be rendered
	like cameras or light, we would have to call
	SceneManager->registerNodeForRendering(this, SNRT_LIGHT_AND_CAMERA);
	After this, we call the actual
	irr::scene::ISceneNode::OnRegisterSceneNode() method of the base class,
	which simply lets also all the child scene nodes of this node register
	themselves.
	*/
	virtual void OnRegisterSceneNode()
	{
		if (IsVisible)
			SceneManager->registerNodeForRendering(this);

		ISceneNode::OnRegisterSceneNode();
	}

	/*
	In the render() method most of the interesting stuff happens: The
	Scene node renders itself. We override this method and draw the
	tetraeder.
	*/
	virtual void render()
	{
		short facefactor=0;
		if(doOuter)	++facefactor;	if(doInner)	++facefactor;

		u16 indices[(ndivs+1)*4*facefactor];

		for(short i=0;i<(ndivs+1);i++)
			{
			short ii=i*4*facefactor;
			short iii;
			short offset=0;

			// get a clockwise rotation	-- outward faces
			if(doOuter)
				{	iii=ii+offset;
				indices[iii]=i;
				indices[iii+1]=i+1;
				indices[iii+2]=i+(ndivs+1)+1;
				indices[iii+3]=i+(ndivs+1);
				offset+=4;
				}

			#if 1
			// get a counterclockwise rotation	-- inward faces
			if(doInner)
				{	iii=ii+offset;
				indices[iii]=i+(ndivs+1);
				indices[iii+1]=i+(ndivs+1)+1;
				indices[iii+2]=i+1;
				indices[iii+3]=i;
				offset+=4;
				}
			#endif
			}


		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(
						&vertices[0], (ndivs+1)*2, &indices[0], ndivs*facefactor
						, video::EVT_STANDARD, scene::EPT_QUADS
						, video::EIT_16BIT);
	}

	/*
	And finally we create three small additional methods.
	irr::scene::ISceneNode::getBoundingBox() returns the bounding box of
	this scene node, irr::scene::ISceneNode::getMaterialCount() returns the
	amount of materials in this scene node (our tetraeder only has one
	material), and irr::scene::ISceneNode::getMaterial() returns the
	material at an index. Because we have only one material here, we can
	return the only one material, assuming that no one ever calls
	getMaterial() with an index greater than 0.
	*/
	virtual const core::aabbox3d<f32>& getBoundingBox() const
	{
		return Box;
	}

	virtual u32 getMaterialCount() const
	{
		return 1;
	}

	virtual video::SMaterial& getMaterial(u32 i)
	{
		return Material;
	}
};



