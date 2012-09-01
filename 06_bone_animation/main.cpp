/**
Bone and Hierarchy Animation Test
**/

#ifdef _MSC_VER
// We'll also define this to stop MSVC complaining about sprintf().
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include "driverChoice.h"

using namespace irr;

#include "pipe.h"

/*
To receive events like mouse and keyboard input, or GUI events like "the OK
button has been clicked", we need an object which is derived from the
irr::IEventReceiver object. There is only one method to override:
irr::IEventReceiver::OnEvent(). This method will be called by the engine once
when an event happens. What we really want to know is whether a key is being
held down, and so we will remember the current state of each key.
*/
class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


/*
The event receiver for keeping the pressed keys is ready, the actual responses
will be made inside the render loop, right before drawing the scene. So lets
just create an irr::IrrlichtDevice and the scene node we want to move. We also
create some other additional scene nodes, to show that there are also some
different possibilities to move and animate scene nodes.
*/
int main()
{
	// ask user for driver
	////video::E_DRIVER_TYPE driverType=driverChoiceConsole();
	video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;
	if (driverType==video::EDT_COUNT)
		return 1;

	// create device
	MyEventReceiver receiver;

	IrrlichtDevice* device = createDevice(driverType,
			core::dimension2d<u32>(640, 480), 16, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();
	char teximgstr[]="stones.jpg";


	//define spheres
	const float radius=15.0;
	const short nnodes=9;
	const short rootidx=nnodes/2;
	#if 0
		scene::ISceneNode*nodes[nnodes];
		scene::ISceneNode * node = smgr->addSphereSceneNode(radius,8,0,1);
		if (node)
			{	node->setPosition(core::vector3df(0,0,30));
			node->setMaterialTexture(0, driver->getTexture(teximgstr));
			node->setMaterialFlag(video::EMF_LIGHTING, false);
			}
		nodes[rootidx]=node;
	#elif 1
		scene::ISceneNode*nodes[nnodes];
		for(short i=0;i<nnodes;i++)
			if(nodes[i]=smgr->addSphereSceneNode(radius,12,0,i))
				{
				nodes[i]->setMaterialTexture(0, driver->getTexture(teximgstr));
				nodes[i]->setMaterialFlag(video::EMF_LIGHTING, false);
				if(i==rootidx)	{	nodes[i]->setPosition(core::vector3df(0,0,30));
					nodes[i]->setScale(core::vector3df(1.0,1.0,1.0));	}
				else if(i<rootidx)	nodes[i]->setPosition(core::vector3df(0,-radius*2,0));
				else if(i>rootidx)	nodes[i]->setPosition(core::vector3df(0,+radius*2,0));
				}
		for(short i=0;i<nnodes;i++)
			if(i==rootidx);
			else if(i<rootidx)	nodes[i]->setParent(nodes[i+1]);
			else if(i>rootidx)	nodes[i]->setParent(nodes[i-1]);
	#elif 0
		ShapeNode*nodes[nnodes];
		for(short i=0;i<nnodes;i++)
			if(nodes[i]=new ShapeNode(smgr->getRootSceneNode(),smgr,1,radius))
				{
				nodes[i]->setMaterialTexture(0, driver->getTexture(teximgstr));
				nodes[i]->setMaterialFlag(video::EMF_LIGHTING, false);
				if(i==rootidx)	{	nodes[i]->setPosition(core::vector3df(0,0,30));
					nodes[i]->setScale(core::vector3df(1.0,1.0,1.0));	}
				else if(i<rootidx)	nodes[i]->setPosition(core::vector3df(0,-radius*2,0));
				else if(i>rootidx)	nodes[i]->setPosition(core::vector3df(0,+radius*2,0));
				}
		for(short i=0;i<nnodes;i++)
			if(i==rootidx);
			else if(i<rootidx)	nodes[i]->setParent(nodes[i+1]);
			else if(i>rootidx)	nodes[i]->setParent(nodes[i-1]);
	#elif 0
		PipeNode*nodes[nnodes];
		for(short i=0;i<nnodes;i++)
			if(nodes[i]=new PipeNode(smgr->getRootSceneNode(),smgr,i,radius,8
										,video::SColor(127,0,255,0),radius*2,true,true))
				{
				//nodes[i]->setMaterialTexture(0, driver->getTexture(teximgstr));
				nodes[i]->setMaterialFlag(video::EMF_LIGHTING, false);
				if(i==rootidx)	{	nodes[i]->setPosition(core::vector3df(0,0,30));
					nodes[i]->setScale(core::vector3df(1.0,1.0,1.0));	}
				else if(i<rootidx)	nodes[i]->setPosition(core::vector3df(0,-radius*2,0));
				else if(i>rootidx)	nodes[i]->setPosition(core::vector3df(0,+radius*2,0));
				}
		for(short i=0;i<nnodes;i++)
			if(i==rootidx);
			else if(i<rootidx)	nodes[i]->setParent(nodes[i+1]);
			else if(i>rootidx)	nodes[i]->setParent(nodes[i-1]);
	#endif


	/*
	To be able to look at and move around in this scene, we create a first
	person shooter style camera and make the mouse cursor invisible.
	*/
	smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	u32 then = device->getTimer()->getTime();

	const f32 MOVEMENT_SPEED = 5.f;

	float rotdeg=0;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		/* Check if keys W, S, A or D are being held down, and move the
		sphere node around respectively. */
		core::vector3df nodePosition = nodes[rootidx]->getPosition();

		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_S))
			nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

		if(receiver.IsKeyDown(irr::KEY_KEY_A))
			nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
		else if(receiver.IsKeyDown(irr::KEY_KEY_D))
			nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;

		nodes[rootidx]->setPosition(nodePosition);

		driver->beginScene(true, true, video::SColor(255,113,113,133));

		for(short i=0;i<nnodes;i++)
			if(i==rootidx||i==0||i==nnodes-1)	;
			else if(abs(i-rootidx)==1)	nodes[i]->setRotation(core::vector3df(rotdeg,0,90));
			else if(abs(i-rootidx)==2)	nodes[i]->setRotation(core::vector3df(rotdeg,0,90));
			else if(abs(i-rootidx)==3)	nodes[i]->setRotation(core::vector3df(rotdeg,0,90));
		#if 1
		rotdeg+=0.1;
		#else
		rotdeg=0;
		#endif

		smgr->drawAll(); // draw the 3d scene
		device->getGUIEnvironment()->drawAll(); // draw the gui environment (the logo)

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	/*
	In the end, delete the Irrlicht device.
	*/
	device->drop();

	return 0;
}

/*
That's it. Compile and play around with the program.
**/
