#if 1

#include "irrlicht.h"

#include "Track.h"
#include "TrackFactory.h"
#include "TrackMesh.h"
#include "TrackOperator.h"
#include "TrainFactory.h"
#include "unitconv.h"
#include "MyEventReceiver.h"
#include <sstream>
#include <cstdlib>
#include <cmath>


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

   IGUIEnvironment* guienv = device->getGUIEnvironment();

   IGUIFont*font=guienv->getBuiltInFont();
   vector<IGUIStaticText*> msgary;
   const int amtmsgs=8;
   msgary.clear();
   {
   int y=0;
   for(int i=0;i<amtmsgs;i++)
		{
		msgary.push_back(guienv->addStaticText(L"text line",
                				rect<s32>(0,y,160,y+12), true)		);
		y+=12;
		}
   }
	for(int i=0;i<msgary.size();i++)
		{
		msgary[i]->setBackgroundColor(SColor(127,255,255,255));
		msgary[i]->setDrawBackground(true);
		msgary[i]->setDrawBorder(true);
		msgary[i]->setWordWrap(true);
		}

	//set up timer:
	ITimer*timer;
	timer=device->getTimer( );

	timer->setSpeed(1);
	timer->setTime(0);
	timer->start();

	u32 thistime,lasttime;
	float timeElapsed;
	thistime=timer->getTime();

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
	sky->setScale(vector3df(10.0,10.0,10.0));

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


	#define doterrain 1
	#if doterrain
	ITerrainSceneNode*ter;
	ter=smgr->addTerrainSceneNode("./terrain/heightmap.png"
				,0
				,-1
				,vector3df(0.0,0.0,0.0)
				,vector3df(0.0,0.0,0.0)
				,vector3df(10.0,10.0,10.0)
				,video::SColor(255,67,100,11)	//green
				////,video::SColor(255,67,11,100)	//purple
				);

	vector3df tercenter,terpos;
	tercenter=ter->getTerrainCenter();
	tercenter.Y=0.0;
	terpos=ter->getPosition();
	terpos-=tercenter;
	ter->setPosition(terpos);
	#endif

	CreateTrackA:

		//Create the track and track mesh:
		Track*track;
		track=TrackFactory::getinstance()->createTestTrack(vector3df(0.0,1.0,-100.0));
		TrackMesh mesh(track,smgr->getSceneCollisionManager(),driver);

		mesh.init(track,1.0,driver);

		// Add the mesh to the scene graph:
		IMeshSceneNode* meshnode = smgr -> addMeshSceneNode(mesh.mesh);
		meshnode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
		meshnode->setMaterialFlag(video::EMF_POINTCLOUD, false);
		meshnode->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
		meshnode->setMaterialFlag(video::EMF_COLOR_MATERIAL,ECM_DIFFUSE_AND_AMBIENT);
		meshnode->getMaterial(0).AmbientColor.set(255,255,0,0);

	CreateTrackOperatorA:
		TrackOperator*trackop;
		trackop=new TrackOperator(track);
		trackop->minspeed=miles_per_hour_to_meter_per_second*7.0f;
		trackop->trains.clear();
		trackop->blocks.clear();
		trackop->trains.push_back(TrainFactory::getinstance()
		                          ->createATestTrain());
		trackop->autolift=false;
		trackop->AddBlock(0.0f,145.0f,true);

	CreateTrackB:
		//Create the track and track mesh:
		deque<Track*> trackB;
		deque<TrackMesh*> meshB;
		deque<IMeshSceneNode*> meshnodeB;

		int n=0;
		for(int i=0;i<n;i++)
			{
			float x=200.0*(float)(i+1);
			trackB.push_back(TrackFactory::getinstance()->createTestTrack(vector3df(x,5.0,0.0)));
			meshB.push_back(new TrackMesh(trackB.back(),smgr->getSceneCollisionManager(),driver));
			meshB.back()->init(trackB.back(),1.0,driver);

			// Add the mesh to the scene graph:
			meshnodeB.push_back(smgr -> addMeshSceneNode(meshB.back()->mesh));
			meshnodeB.back()->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
			meshnodeB.back()->setMaterialFlag(video::EMF_POINTCLOUD, false);
			meshnodeB.back()->setMaterialFlag(video::EMF_GOURAUD_SHADING, true);
			meshnodeB.back()->setMaterialFlag(video::EMF_COLOR_MATERIAL,ECM_DIFFUSE_AND_AMBIENT);
			meshnodeB.back()->getMaterial(0).AmbientColor.set(255,255,0,0);
			}

	// light is just for nice effects

	ILightSceneNode *node = smgr->addLightSceneNode(0, vector3df(0,0,1000000),
									SColorf(1.0f,1.0f,1.0f,1.0f), 1.0f);
	node->setLightType(ELT_DIRECTIONAL);
	node->setRotation(vector3df(60.0,0.0,0.0));
	/*
	ILightSceneNode *backlight = smgr->addLightSceneNode(0, vector3df(0,0,1000000),
									SColorf(1.0f,0.1f,0.1f,0.1f), 1.0f);
	backlight->setLightType(ELT_DIRECTIONAL);
	backlight->setRotation(vector3df(-60.0,0.0,0.0));
	*/

	#define do_FPS 0
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
		camera->setNearValue(0.05f);
		}

	/*
	Just a usual render loop with event handling. The custom mesh is
	a usual part of the scene graph which gets rendered by drawAll.
	*/
	f32 trackpos=0.0f;
	f32 trackpos_inc=0.01f;
	f32 tracklen=track->getTrackLen();
	f32 trainspeed=0.0f;

	while(device->run())
		{
		if(!device->isWindowActive())
			{
			device->sleep(100);
			continue;
			}

		meshnode->setMaterialFlag(video::EMF_WIREFRAME, false);
		for(int i=0;i<meshnodeB.size();i++)
			meshnodeB.at(i)->setMaterialFlag(video::EMF_WIREFRAME, false);
		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			{
			////meshnode->setMaterialFlag(video::EMF_WIREFRAME, !meshnode->getMaterial(0).Wireframe);
			meshnode->setMaterialFlag(video::EMF_WIREFRAME, true);
			for(int i=0;i<meshnodeB.size();i++)
				meshnodeB.at(i)->setMaterialFlag(video::EMF_WIREFRAME, true);
			}

		UpdateTime:
			lasttime=thistime;
			thistime=timer->getTime();
			timeElapsed=thistime-lasttime;
			timeElapsed/=1000.0f;

		ModifySpeed:
			const float max_speed=30.0f;
			float accel=0.0;
			float speedmul;
			speedmul=1.0f;
			speedmul*=(receiver.IsKeyDown(irr::KEY_LSHIFT)||receiver.IsKeyDown(irr::KEY_RSHIFT))?	10.0f	: 1.0;
			speedmul*=(receiver.IsKeyDown(irr::KEY_LCONTROL)||receiver.IsKeyDown(irr::KEY_RCONTROL))?	0.01f	: 1.0;
			accel=0.001f*speedmul;
			if(receiver.IsKeyDown(irr::KEY_ADD)||receiver.IsKeyDown(irr::KEY_PLUS))
				trackpos_inc=(trackpos_inc+accel<max_speed)?	trackpos_inc+accel : max_speed;
			else if(receiver.IsKeyDown(irr::KEY_MINUS)||receiver.IsKeyDown(irr::KEY_SUBTRACT))
				trackpos_inc=(trackpos_inc-accel>-max_speed)?	trackpos_inc-accel : -max_speed;


		if(receiver.IsKeyDown(irr::KEY_KEY_S))
			{
			trackpos_inc=0;
			////'device->sleep(10);
			}

		#if !do_FPS

		float raisemul=speedmul;

		static f32 headht=0.75;		//0.75 meters off the track
		////static float headht=0.2;

		if(receiver.IsKeyDown(irr::KEY_KEY_0))
			{
			timer->setSpeed(0.0f);
			}
		if(receiver.IsKeyDown(irr::KEY_KEY_1))
			{
			timer->setSpeed(1.0f);
			}
		if(receiver.IsKeyDown(irr::KEY_KEY_2))
			{
			timer->setSpeed(4.0f);
			}
		if(receiver.IsKeyDown(irr::KEY_KEY_3))
			{
			timer->setSpeed(16.0f);
			}
		if(receiver.IsKeyDown(irr::KEY_UP))
			{
			//inverted coaster
			headht+=raisemul*0.0001f;
			}
		if(receiver.IsKeyDown(irr::KEY_DOWN))
			{
			//inverted coaster
			headht-=raisemul*0.0001f;
			}

		if(receiver.IsKeyDown(irr::KEY_KEY_I))
			{
			//inverted coaster
			headht=-std::abs((float)headht);
			}
		else
			{
			headht=std::abs((float)headht);
			}

		if(camera)
			{
			SetOrientation:
				Orientation ori;
				#if 0	//manual speed control:
					ori=track->getori( trackpos);
					trackpos+=trackpos_inc;
					trackpos=fmod(trackpos,tracklen);
					if(trackpos<0.0)	trackpos+=tracklen;
				#else	//Track operator and physics:
					trackop->MoveTrain(0,timeElapsed);
					Car&car=trackop->trains.at(0)->cars.at(0);
					ori=car.ori;
					trainspeed=trackop->trains.at(0)->speed;
					trackpos=trackop->trains.at(0)->cars.at(0).linpos;
				#endif
			SetCamera:
				ori.pos=ori.pos+headht*ori.hdg.getup();
				camera->setPosition(ori.pos);
				camera->setTarget(ori.pos+ori.hdg.getfwd());
				camera->setUpVector(ori.hdg.getup());
				camera->setFarValue(20000.0f);
				camera->setNearValue(0.05f);
			DisplayInfo:
				if(timer->getTime()%20==0)
					{
					int i=0;
					stringstream ss;
					char s[64];
					wchar_t wcs[64];
					////memset(s,0,sizeof(s));
					for(int i=0;i<msgary.size();i++)
						{
						ss.str("");
						switch(i)
							{
							case 0:	ss<<"tracklen="<<tracklen; break;
							case 1:	ss<<"trackpos="<<trackpos;	break;
							case 2:	ss<<"pos_inc="<<trackpos_inc;	break;
							case 3:	ss<<"train speed (m/s)="<<trainspeed;	break;
							case 4:	ss<<"train speed (mph)="<<(trainspeed/miles_per_hour_to_meter_per_second);	break;
							case 5: case 6: case 7:
								{
								vector3df&gf=trackop->trains.at(0)->cars.at(0).gforces;
								if(i==5)			ss<<"lat g's:"<<gf.X;
								else if(i==6)	ss<<"vrt g's:"<<gf.Y;
								else if(i==7)	ss<<"acc g's:"<<gf.Z;
								break;
								}
							default: ss<<"___________";	break;
							}
						strcpy(s,ss.str().c_str());
						memset(wcs,0,sizeof(wcs));
						mbstowcs (wcs,s,strlen(s));
						msgary[i]->setText(wcs);
						}
					}
			}
		#endif

		if(receiver.IsKeyDown(irr::KEY_KEY_T))
			{
			//top-down view:
				camera->setPosition(vector3df(0.0f,200.0f,0.0f));
				camera->setTarget(vector3df(0.0f,0.0f,0.0f));
				////camera->setFarValue(20000.0f);
				////camera->setNearValue(0.05f);
			////'device->sleep(10);
			}

		node->getLightData().AmbientColor.set(0.0,0.0,0.0);

		driver->beginScene(true, true, SColor(0xff000000));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();
		}

	device->drop();

	return 0;
	}

/*
That's it! Just compile and play around with the program.
**/

#endif








