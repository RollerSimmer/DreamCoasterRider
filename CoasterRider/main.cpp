/** Example 003 Custom SceneNode

This Tutorial is more advanced than the previous ones.
If you are currently just playing around with the Irrlicht
engine, you may want to look at other examples first.
This tutorials shows how to create a custom scene node and
how to use it in the engine. A custom scene node is needed
if you want to implement a render technique the Irrlicht
Engine currently does not support. For example, you can write
an indoor portal based renderer or an advanced terrain scene
node with it. By creating custom scene nodes, you can
easily extend the Irrlicht Engine and adapt it to your own
needs.

I will keep the tutorial simple: Keep everything very
short, everything in one .cpp file, and I'll use the engine
here as in all other tutorials.

To start, I include the header files, use the irr namespace,
and tell the linker to link with the .lib file.
*/
#include <irrlicht.h>
#include "driverChoice.h"
#include <cmath>
#include <vector>
using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

static int nsides;	//used for the poly functions


/*
Here comes the more sophisticated part of this tutorial:
The class of our very own custom scene node. To keep it simple,
our scene node will not be an indoor portal renderer nor a terrain
scene node, but a simple tetraeder, a 3d object consisting of 4
connected vertices, which only draws itself and does nothing more.
Note that this scenario does not require a custom scene node in Irrlicht.
Instead one would create a mesh from the geometry and pass it to a
irr::scene::IMeshSceneNode. This example just illustrates creation of a custom
scene node in a very simple setting.

To let our scene node be able to be inserted into the Irrlicht
Engine scene, the class we create needs to be derived from the
irr::scene::ISceneNode class and has to override some methods.
*/

class CSampleSceneNode : public scene::ISceneNode
{

	/*
	First, we declare some member variables:
	The bounding box, 4 vertices, and the material of the tetraeder.
	*/
	core::aabbox3d<f32> Box;
	video::S3DVertex Vertices[36];
	s16 nverts;
	video::SMaterial Material;

	/*
	The parameters of the constructor specify the parent of the scene node,
	a pointer to the scene manager, and an id of the scene node.
	In the constructor we call the parent class' constructor,
	set some properties of the material, and
	create the 4 vertices of the tetraeder we will draw later.
	*/

public:

	CSampleSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id
							,core::vector3d<float>P[]
							,core::vector3d<float>Ups[]
							,short ncontrolpts)
		: scene::ISceneNode(parent, mgr, id)
	{

		Material.Wireframe = true;
		Material.Lighting = false;

		std::vector<core::vector3d<float> >p;
		core::vector3d<float> basetri[3]
			=	{{-0.5,0,0},{0.5,0,0},{0.25,-0.25,0}};
		core::vector3d<float> rottri[2];

		core::vector3d<float> A,B,C;
		float ABdot,BCdot,ABmag,BCmag;
		float cosABang,cosBCang,ABang,BCang,angtotal;

		//calculate angle of rotation
		{
		A=P[1]-P[0];
		B=P[2]-P[1];
		C=P[3]-P[2];
		ABdot=A.dotProduct(B);
		BCdot=B.dotProduct(C);
		ABmag=A.getLength()*B.getLength();
		BCmag=B.getLength()*C.getLength();
		cosABang=ABdot/ABmag;
		cosBCang=BCdot/BCmag;
		ABang=acos(cosABang);
		BCang=acos(cosBCang);
		angtotal=ABang+BCang;
		}

		//set up the interpolation of the spline
		const float pi=3.1415927;
		float angdegrees=angtotal*180/pi;
		float npoints=angdegrees/12;
		nverts=(s16)(npoints+0.5);
		float tstep=1/std::max((float)nverts,1.0f);
		core::vector3d<float> curpt;
		core::vector3d<float> curup;

		//print some info before setting the interppolation points
		std::cout<<"total curve angle: "<<angtotal<<std::endl;
		std::cout<<"; steps: "<<nverts<<std::endl;

		//interpolate the spline
		p.clear();
		for(float t=0.0;t<1.0+tstep;t+=tstep)
			{
			float scale[4]=
				{
				pow(1.0f-t,3)
				,3*pow(1.0f-t,2)*t
				,3*(1.0f-t)*pow(t,2)
				,pow(t,3)
				};
			curup= 	scale[0]*Ups[0]				//pow(1-t,3)*P[0]
						+	scale[1]*Ups[1]		//3*pow(1-t,2)*t*P[1]
						+	scale[2]*Ups[2]	//3*(1-t)*pow(t,2)*P[2]
						+	scale[3]*Ups[3];					//pow(t,3)*P[3]
			curpt= 	scale[0]*P[0]				//pow(1-t,3)*P[0]
						+	scale[1]*P[1]		//3*pow(1-t,2)*t*P[1]
						+	scale[2]*P[2]	//3*(1-t)*pow(t,2)*P[2]
						+	scale[3]*P[3];					//pow(t,3)*P[3]
			//=(1-$A11)^3*B$4    +      3*(1-$A11)^2*$A11*B$5
			//+     3*(1-$A11)*$A11^2*B$6    +    $A11^3*B$7
			p.push_back(curpt);
			}
		for(int i=0;i<p.size();i++)
			{
			std::cout<<"interpolation - pt "<<i<<" - ";
			std::cout<<"("<<p[i].X<<","<<p[i].Y<<","<<p[i].Z<<")"<<std::endl;

			Vertices[i] =
					video::S3DVertex(p[i].X,p[i].Y,p[i].Z, 1,1,0,
							video::SColor(255,0,0,255), 0, 1);
			}

	/*
	The Irrlicht Engine needs to know the bounding box of a scene node.
	It will use it for automatic culling and other things. Hence, we
	need to create a bounding box from the 4 vertices we use.
	If you do not want the engine to use the box for automatic culling,
	and/or don't want to create the box, you could also call
	irr::scene::ISceneNode::setAutomaticCulling() with irr::scene::EAC_OFF.
	*/
		Box.reset(Vertices[0].Pos);
		for (s32 i=1; i<nverts; ++i)
			Box.addInternalPoint(Vertices[i].Pos);
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
	/*
	01 45 8 A
	23	67 9 B

	18 5A
	39 7B

	014501
	236723
	*/
		u16 indices[36];

		for(int i=0;i<nverts;i++)
			indices[i]=i;

		video::IVideoDriver* driver = SceneManager->getVideoDriver();

		driver->setMaterial(Material);
		driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
		driver->drawVertexPrimitiveList(&Vertices[0], nverts, &indices[0], nverts-1, video::EVT_STANDARD, scene::EPT_LINE_STRIP, video::EIT_16BIT);
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

/*
That's it. The Scene node is done. Now we simply have to start
the engine, create the scene node and a camera, and look at the result.
*/
int mainA(int npars,char*pars[])
{
	if(npars>1)
		{
		nsides=atoi(pars[1]);
		std::cout<<"parameter read - "<<nsides<<std::endl;
		}
	else
		nsides=1;


	// set the driver to open GL
		video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;

	// create device

		IrrlichtDevice *device = createDevice(driverType,
				core::dimension2d<u32>(640, 480), 16, false);

		if (device == 0)
			return 1; // could not create selected driver.

	// create engine and camera

		device->setWindowCaption(L"Coaster Track Demo - Irrlicht");

		video::IVideoDriver* driver = device->getVideoDriver();
		scene::ISceneManager* smgr = device->getSceneManager();

		irr::scene::ICameraSceneNode*camera;
		#if 0
			camera=smgr->addCameraSceneNode(0, core::vector3df(0,-40,0), core::vector3df(0,0,0));
		#else
			SKeyMap keyMap[8];
			keyMap[0].Action = EKA_MOVE_FORWARD;
			keyMap[0].KeyCode = KEY_UP;
			keyMap[1].Action = EKA_MOVE_FORWARD;
			keyMap[1].KeyCode = KEY_KEY_W;

			keyMap[2].Action = EKA_MOVE_BACKWARD;
			keyMap[2].KeyCode = KEY_DOWN;
			keyMap[3].Action = EKA_MOVE_BACKWARD;
			keyMap[3].KeyCode = KEY_KEY_S;

			keyMap[4].Action = EKA_STRAFE_LEFT;
			keyMap[4].KeyCode = KEY_LEFT;
			keyMap[5].Action = EKA_STRAFE_LEFT;
			keyMap[5].KeyCode = KEY_KEY_A;

			keyMap[6].Action = EKA_STRAFE_RIGHT;
			keyMap[6].KeyCode = KEY_RIGHT;
			keyMap[7].Action = EKA_STRAFE_RIGHT;
			keyMap[7].KeyCode = KEY_KEY_D;
			camera = smgr->addCameraSceneNodeFPS(0, 100, 500, -1, keyMap, 8);
			#endif



	//initialize the control points
		core::vector3d<float> controlpts[4];
		core::vector3d<float> ups[4];
		#if 1
		controlpts[0]=core::vector3d<float>(0.0f,0.0f,0.0f);
		controlpts[1]=core::vector3d<float>(0.0f,0.0f,20.0f);
		controlpts[2]=core::vector3d<float>(0.0f,20.0f,20.0f);
		controlpts[3]=core::vector3d<float>(20.0f,20.0f,20.0f);
		#else
		controlpts[0]=core::vector3d<float>(20.0f,0.0f,0.0f);
		controlpts[1]=core::vector3d<float>(20.0f,10.0f,0.0f);
		controlpts[2]=core::vector3d<float>(10.0f,20.0f,0.0f);
		controlpts[3]=core::vector3d<float>(0.0f,20.0f,0.0f);
		#endif
		{
		int i=0;
		ups[i]=core::vector3d<float>(0,1,0);	ups[i].normalize();	i++;
		ups[i]=core::vector3d<float>(0,0,-1);	ups[i].normalize();	i++;
		ups[i]=core::vector3d<float>(1,0,0);	ups[i].normalize();	i++;
		ups[i]=core::vector3d<float>(0,-1,0);	ups[i].normalize();	i++;
		}


	//**********************************
	//set up the quaternion for rotation
	//**********************************
		float axisangle=45;	//angle of rotation in degrees
				////float axisangle=90/12;	//angle of rotation in degrees
		float axisradians=axisangle*3.1415927/180;
		float axishalfradians=axisradians/2;
		core::vector3df axis(0,1,0);
		axis.normalize();
		core::quaternion q;
		q.fromAngleAxis(axisradians,axis);
		q.W=-q.W;
		core::quaternion qinv=q;
		qinv.makeInverse();

	//set up the point quat
		core::quaternion qpoint(1,0,0,0);

	//do rotation sequence
		core::quaternion qfinal;
		qfinal=qinv*qpoint;
		qfinal=qfinal*q;	//this is the final step and gives you the rotated point

	//print the results of the custom axis rotation
		std::cout<<"******** Custom Axis Rotation ********"<<std::endl;
		std::cout<<"\tAxis: <"<<axis.X<<","<<axis.Y<<","<<axis.Z<<">"<<std::endl;
		std::cout<<"\tAngle(deg): "<<axisangle<<std::endl;
		std::cout<<"\tP: <"<<qpoint.X<<","<<qpoint.Y<<","<<qpoint.Z<<">"<<std::endl;
		std::cout<<"\tP': <"<<qfinal.X<<","<<qfinal.Y<<","<<qfinal.Z<<">"<<std::endl;
		std::cout<<"******** /Custom Axis Rotation ********"<<std::endl;

	/*
	Create our scene node. I don't check the result of calling new, as it
	should throw an exception rather than returning 0 on failure. Because
	the new node will create itself with a reference count of 1, and then
	will have another reference added by its parent scene node when it is
	added to the scene, I need to drop my reference to it. Best practice is
	to drop it only *after* I have finished using it, regardless of what
	the reference count of the object is after creation.
	*/
		CSampleSceneNode *myNode =
			new CSampleSceneNode(smgr->getRootSceneNode(), smgr, 666,controlpts,ups,4);

	/*
	To animate something in this boring scene consisting only of one
	tetraeder, and to show that you now can use your scene node like any
	other scene node in the engine, we add an animator to the scene node,
	which rotates the node a little bit.
	irr::scene::ISceneManager::createRotationAnimator() could return 0, so
	should be checked.
	*/
	scene::ISceneNodeAnimator* anim =
		smgr->createRotationAnimator(core::vector3df(0.0f, 0.0f, 0.0f));

	if(anim)
	{
		myNode->addAnimator(anim);

	/*
	I'm done referring to anim, so must
	irr::IReferenceCounted::drop() this reference now because it
	was produced by a createFoo() function. As I shouldn't refer to
	it again, ensure that I can't by setting to 0.
	*/
		anim->drop();
		anim = 0;
	}

	/*
	I'm done with my CSampleSceneNode object, and so must drop my reference.
	This won't delete the object, yet, because it is still attached to the
	scene graph, which prevents the deletion until the graph is deleted or the
	custom scene node is removed from it.
	*/
		myNode->drop();
		myNode = 0; // As I shouldn't refer to it again, ensure that I can't

	/*
	Now draw everything and finish.
	*/
		u32 frames=0;
		while(device->run())
		{
			driver->beginScene(true, true, video::SColor(0,100,100,100));

			smgr->drawAll();

			driver->endScene();
			if (++frames==100)
			{
				core::stringw str = L"Irrlicht Engine [";
				str += driver->getName();
				str += L"] FPS: ";
				str += (s32)driver->getFPS();

				device->setWindowCaption(str.c_str());
				frames=0;
			}
		}

		device->drop();

		return 0;
}

/*
That's it. Compile and play around with the program.
**/
