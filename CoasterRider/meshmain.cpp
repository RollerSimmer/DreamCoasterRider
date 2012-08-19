#if 1

#include "irrlicht.h"

#include "Track.h"
#include "TrackMesh.h"
#include "MyEventReceiver.h"

/*
Much of this is code taken from some of the examples. We merely set
up a mesh from a heightmap, light it with a moving light, and allow
the user to navigate around it.
*/

int main(int argc, char* argv[])
	{
	// ask user for driver
	////video::E_DRIVER_TYPE driverType=driverChoiceConsole();

	////if(driverType==video::EDT_COUNT)
	////	return 1;
	video::E_DRIVER_TYPE driverType=video::EDT_OPENGL;

	MyEventReceiver receiver;
	IrrlichtDevice* device = createDevice(driverType,
													  core::dimension2du(640,480), 32, false, false, false,
													  &receiver);

	if(device == 0)
		return 1;


	IVideoDriver *driver = device->getVideoDriver();
	ISceneManager *smgr = device->getSceneManager();
	device->setWindowCaption(L"Coaster Rider");

	//add a sky
	/*
	virtual ISceneNode* irr::scene::ISceneManager::addSkyBoxSceneNode 	( 	video::ITexture *  	top,
		video::ITexture *  	bottom,
		video::ITexture *  	left,
		video::ITexture *  	right,
		video::ITexture *  	front,
		video::ITexture *  	back,
		ISceneNode *  	parent = 0,
		s32  	id = -1
	)
	smgr->addSkyBoxSceneNode(
				 driver->getTexture("./images/skytop.jpg")
				,driver->getTexture("./images/skybtm.jpg")
				,driver->getTexture("./images/skylft.jpg")
				,driver->getTexture("./images/skyrgt.jpg")
				,driver->getTexture("./images/skyfwd.jpg")
				,driver->getTexture("./images/skybak.jpg")
				);
	virtual ISceneNode* irr::scene::ISceneManager::addSkyDomeSceneNode 	( 	video::ITexture *  	texture,
			u32  	horiRes = 16,
			u32  	vertRes = 8,
			f32  	texturePercentage = 0.9,
			f32  	spherePercentage = 2.0,
			f32  	radius = 1000.f,
			ISceneNode *  	parent = 0,
			s32  	id = -1
		)
	*/

	ISceneNode*sky;
	sky=smgr->addSkyDomeSceneNode(driver->getTexture("./textures/skydome.jpg"));
	sky->setPosition(vector3df(0.0,0.0,0.0));
	sky->setScale(vector3df(20.0,20.0,20.0));

	//add a terrain:
	#if 0
	virtual ITerrainSceneNode* irr::scene::ISceneManager::addTerrainSceneNode 	( 	const io::path &  	heightMapFileName,
		ISceneNode *  	parent = 0,
		s32  	id = -1,
		const core::vector3df &  	position = core::vector3df(0.0f, 0.0f, 0.0f),
		const core::vector3df &  	rotation = core::vector3df(0.0f, 0.0f, 0.0f),
		const core::vector3df &  	scale = core::vector3df(1.0f, 1.0f, 1.0f),
		video::SColor  	vertexColor = video::SColor(255, 255, 255, 255),
		s32  	maxLOD = 5,
		E_TERRAIN_PATCH_SIZE  	patchSize = ETPS_17,
		s32  	smoothFactor = 0,
		bool  	addAlsoIfHeightmapEmpty = false
	)
	#endif

	ITerrainSceneNode*ter;
	ter=smgr->addTerrainSceneNode("./terrain/heightmap.png"
				,0
				,-1
				,vector3df(0.0,0.0,0.0)
				,vector3df(0.0,0.0,0.0)
				,vector3df(20.0,2.0,20.0)
				,video::SColor(255,67,100,11)	//green
				////,video::SColor(255,67,11,100)	//purple
				);

	vector3df tercenter,terpos;
	tercenter=ter->getTerrainCenter();
	tercenter.Y=0.0;
	terpos=ter->getPosition();
	terpos-=tercenter;
	ter->setPosition(terpos);

	/*
	Create the custom mesh and initialize with a heightmap
	*/
	Track track;
	////track.load("track.xml");	//load a track
	track.CreateATestTrack();
	TrackMesh mesh(&track);
	mesh.init(&track,1.0,driver);

	// Add the mesh to the scene graph
	IMeshSceneNode* meshnode = smgr -> addMeshSceneNode(mesh.mesh);
	meshnode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	meshnode->setMaterialFlag(video::EMF_POINTCLOUD, false);
	meshnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
	meshnode->setMaterialFlag(video::EMF_COLOR_MATERIAL,ECM_DIFFUSE_AND_AMBIENT);
	meshnode->getMaterial(0).AmbientColor.set(255,255,0,0);
	////meshnode->setMaterialFlag(video::, true);
	////meshnode->getMaterial(0).AmbientColor.color=0xffffffff;


	// light is just for nice effects

	ILightSceneNode *node = smgr->addLightSceneNode(0, vector3df(0,0,1000000),
									SColorf(1.0f,1.0f,1.0f,1.0f), 1.0f);
	node->setLightType(ELT_DIRECTIONAL);
	node->setRotation(vector3df(90.0,0.0,0.0));
	node->enableCastShadow(true);
	////node->setRotation(vector3df(270.0,0.0,0.0));
	////node->getLightData().AmbientColor.set(0.1,0.1,0.1);
	////node->getLightData().AmbientColor.set(255,64,0,0);
	////node->getLightData().AmbientColor.set(1.0,0.0,0.0);
	////ECM_AMBIENT

	#if 0
	if(node)
		{
		node->getLightData().Attenuation.set(0.f, 1.f/500.f, 0.f);
		ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(vector3df(0,150,0),250.0f);

		if(anim)
			{
			node->addAnimator(anim);
			anim->drop();
			}
		}
	#endif

	#define do_FPS 1
	#if do_FPS
		ICameraSceneNode* camera = smgr->
			addCameraSceneNodeFPS
				( 	0,100.0f,0.05f,-1,0,0,false,0.f,false,true);
	#else
		ICameraSceneNode* camera = smgr->addCameraSceneNode();
	#endif


	if(camera)
		{
		camera->setPosition(vector3df(0.f, 25.f, 0.f));
		camera->setTarget(vector3df(0.f, 0.f, 0.f));
		camera->setFarValue(20000.0f);
		}

	/*
	Just a usual render loop with event handling. The custom mesh is
	a usual part of the scene graph which gets rendered by drawAll.
	*/
	f32 trackpos=0.0f;
	f32 trackpos_inc=0.01f;
	f32 tracklen=track.CalcTrackLen(1.0f/3.0f);

	while(device->run())
		{
		if(!device->isWindowActive())
			{
			device->sleep(100);
			continue;
			}

		meshnode->setMaterialFlag(video::EMF_WIREFRAME, false);
		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			{
			////meshnode->setMaterialFlag(video::EMF_WIREFRAME, !meshnode->getMaterial(0).Wireframe);
			if(!meshnode->getMaterial(0).Wireframe)
				meshnode->setMaterialFlag(video::EMF_WIREFRAME, true);
			}

		if(receiver.IsKeyDown(irr::KEY_ADD)||receiver.IsKeyDown(irr::KEY_PLUS))
			{
			if(trackpos_inc<3.f)	trackpos_inc+=0.0001f;
			else trackpos_inc=3.f;
			////device->sleep(10);
			}
		else if(receiver.IsKeyDown(irr::KEY_MINUS)||receiver.IsKeyDown(irr::KEY_SUBTRACT))
			{
			if(trackpos_inc>0.001f)	trackpos_inc-=0.0001f;
			else trackpos_inc=0;
			////'device->sleep(10);
			}

		#if !do_FPS

		f32 headht=0.75;		//0.75 meters off the track

		if(receiver.IsKeyDown(irr::KEY_KEY_I))
			{
			//inverted coaster
			headht=-headht;
			}

		if(camera)
			{
			Track::Orientation ori;
			track.GetHeadingAndPtAt( trackpos,ori.hdg,ori.pos
											,false,false,true);
			ori.pos=ori.pos+headht*ori.hdg.getup();
			camera->setPosition(ori.pos);
			camera->setTarget(ori.pos+ori.hdg.getfwd());
			camera->setUpVector(ori.hdg.getup());
			camera->setFarValue(20000.0f);
			trackpos+=trackpos_inc;
			trackpos=fmod(trackpos,tracklen);
			}
		#endif

		if(receiver.IsKeyDown(irr::KEY_KEY_T))
			{
			//top-down view:
				camera->setPosition(vector3df(0.0f,200.0f,0.0f));
				camera->setTarget(vector3df(0.0f,0.0f,0.0f));
				camera->setFarValue(20000.0f);
			////'device->sleep(10);
			}

		node->getLightData().AmbientColor.set(0.0,0.0,0.0);

		driver->beginScene(true, true, SColor(0xff000000));
		smgr->drawAll();
		driver->endScene();
		}

	device->drop();

	return 0;
	}

/*
That's it! Just compile and play around with the program.
**/

#endif








