#if 1

#include "irrlicht.h"

#include "Track.h"
#include "Orientation.h"
#include "TrackFactory.h"
#include "TrackMesh/TrackMesh.h"
#include "TrackOperator.h"
#include "TrainFactory.h"
#include "unitconv.h"
#include "MyEventReceiver.h"
#include "TrainNodeList.h"

#include "debugclasses/TrackLabeler.h"


////#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;


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
   ////IGUISkin*guiskin=guienv->createSkin(EGST_WINDOWS_CLASSIC);

   IGUISkin*guiskin=guienv->getSkin();
   guienv->setSkin(guiskin);

 	IGUIFont*font=guienv->getFont("./fonts/courier10px.xml");
 	guiskin->setFont(font);

   ////IGUIFont*font=guienv->getBuiltInFont();
 	////IGUIFont*font=guienv->getFont("fonts/courier10px.xml");

   vector<IGUIStaticText*> msgary;
   const int amtmsgs=13;
   msgary.clear();
   {
   int y=0;
   for(int i=0;i<amtmsgs;i++)
		{
		msgary.push_back(guienv->addStaticText(L"text line",
                				rect<s32>(0,y,200,y+12), true)		);
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

	timer->setSpeed(1.0f);
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
	const float	skyscale=0.01;
	sky->setScale(vector3df(skyscale,skyscale,skyscale));

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
				////,video::SColor(255,165,136,108)	//brown
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
		Orientation startori;
		startori.pos.set(0.0,5.0,-100.0);
		////track=TrackFactory::getinstance()->createTestTrack(startori);
		////track=TrackFactory::getinstance()->createCorkscrewTrack(startori);
		////track=TrackFactory::getinstance()->createMegaCoasterTrack(startori);
		////track=TrackFactory::getinstance()->createTopHatCoasterTrack(startori);
		char trackfile[128]="tracks/tornado.xml";
		char sinput[128];
		cout<<"Enter track file to load (XML) (blank for default): "<<endl<<"\t";
		cin>>sinput;
		cout<<endl;
		if(strlen(sinput)>0)
			strcpy(trackfile,sinput);
		track=TrackFactory::getinstance()->createTrackFromFile(trackfile);

	CreateTrackOperatorA:
		TrackOperator*trackop;
		trackop=new TrackOperator(track);
		float forespeedmph,foreaccelmphs,backspeedmph,backaccelmphs,finput;

		trackop->trains.clear();
		trackop->blocks.clear();
		trackop->trains.push_back(TrainFactory::getinstance()
		                          ->createATestTrain());
		float trainpos;
		cout<<"Enter track's operation mode:\n";
		cout<<"\t( 0=laps (default)\n";
		cout<<"\t  1=shuttle back thru station\n";
		cout<<"\t  2=reverse chain drop\n";
		cout<<"\t  3=accel/decel shuttle (stop at station)\n";
		cout<<"\t  4=transport shuttle  )\n";
		int modeint;
		cin>>modeint;
		switch(modeint)
			{
			case 0: trackop->mode=TrackOperator::tom_laps; break;
			case 1: trackop->mode=TrackOperator::tom_shuttle_thru_station; break;
			case 2: trackop->mode=TrackOperator::tom_reverse_chain_drop; break;
			case 3: trackop->mode=TrackOperator::tom_accel_decel_shuttle; break;
			case 4: trackop->mode=TrackOperator::tom_transport_shuttle; break;
			default: trackop->mode=TrackOperator::tom_laps; break;
			}
		trackop->stage=0;

		cout<<"Enter starting track position of trailing car in train: "; cin>>trainpos; cout<<endl;
		cout<<"> You entered: "<<trainpos<<endl;
		trackop->setTrainPos(0,trainpos,true);
		////cout<<"Automatically  the car below min speed even off lift blocks? (1=yes, 0=no) ";
		////cin>>trackop->autolift;	cout<<endl;
		////cout<<"> You entered: "<<trackop->autolift<<endl;

		float blockstart,blocklen;
		bool doaddblock=false;
		bool flag;
		char blocktype[64];
		Block::Type blocktype_enum;
		int blockcount=0;
		do
			{
			if(blockcount==0) cout<<"Add a block? (1=yes, 0=no): ";
			else              cout<<"Add another block? (1=yes, 0=no): ";
			cin>>doaddblock;	cout<<endl;
			cout<<"> You entered: "<<doaddblock<<endl;

			if(doaddblock&&!cin.eof())
				{
				cout<<"-- Block #"<<blockcount<<" --"<<endl;
				cout<<"Block start: "; cin>>blockstart;	cout<<endl;
				cout<<"> You entered: "<<blockstart<<endl;
				cout<<"Block length: "; cin>>blocklen;	cout<<endl;
				cout<<"> You entered: "<<blocklen<<endl;
				cout<<"BlockType? (lowercase choices: normal,trim,lift,boost,station): ";	cin>>blocktype; cout<<endl;
				cout<<"> You entered: "<<blocktype<<endl;
				if(strcmp(blocktype,"normal")==0)
					blocktype_enum=Block::bt_normal;
				else if(strcmp(blocktype,"trim")==0)
					blocktype_enum=Block::bt_trim;
				else if(strcmp(blocktype,"lift")==0)
					blocktype_enum=Block::bt_lift;
				else if(strcmp(blocktype,"boost")==0)
					blocktype_enum=Block::bt_boost;
				else if(strcmp(blocktype,"station")==0)
					blocktype_enum=Block::bt_station;
				else	//default:
					blocktype_enum=Block::bt_normal;

				forespeedmph=10.0;
				cout<<"Enter block forward speed (mph): ";	cin>>finput;	cout<<endl;
				cout<<"> You entered: "<<finput<<endl;
				forespeedmph=finput;

				foreaccelmphs=25.0;
				cout<<"Enter block forward acceleration (mph/s): ";	cin>>finput;	cout<<endl;
				cout<<"> You entered: "<<finput<<endl;
				foreaccelmphs=finput;

				backspeedmph=-10.0;
				cout<<"Enter block backward speed (mph): ";	cin>>finput;	cout<<endl;
				cout<<"> You entered: "<<finput<<endl;
				backspeedmph=finput;

				backaccelmphs=-25.0;
				cout<<"Enter block backward acceleration (mph/s): ";	cin>>finput;	cout<<endl;
				cout<<"> You entered: "<<finput<<endl;
				backaccelmphs=finput;

				float forespeed=miles_per_hour_to_meter_per_second*forespeedmph;
				float foreaccel=miles_per_hour_to_meter_per_second*foreaccelmphs;
				float backspeed=miles_per_hour_to_meter_per_second*backspeedmph;
				float backaccel=miles_per_hour_to_meter_per_second*backaccelmphs;

				trackop->AddBlock(blockstart,blocklen,blocktype_enum
				                  ,forespeed,backspeed
				                  ,foreaccel,backaccel);
				++blockcount;
				}
			}	while(doaddblock);


	CreateTrackLabeler:
		TrackLabeler tlabel(smgr,guienv,trackop);
		tlabel.LabelTrack();


	// light is just for nice effects

	u32
		 railcolor,spinecolor,rungcolor,catwalkcolor
		,handrailcolor,handrungcolor,supportcolor;
	#if 1
	EnterTrackMeshColors:
		{
		cout<<"Enter rail color in hex format (0xRRGGBB): ";
		cin>>hex>>railcolor;
		cout<<"Enter spine color in hex format (0xRRGGBB): ";
		cin>>hex>>spinecolor;
		cout<<"Enter rung color in hex format (0xRRGGBB): ";
		cin>>hex>>rungcolor;
		cout<<"Enter catwalk color in hex format (0xRRGGBB): ";
		cin>>hex>>catwalkcolor;
		cout<<"Enter handrail color in hex format (0xRRGGBB): ";
		cin>>hex>>handrailcolor;
		cout<<"Enter hand-rung color in hex format (0xRRGGBB): ";
		cin>>hex>>handrungcolor;
		cout<<"Enter support color in hex format (0xRRGGBB): ";
		cin>>hex>>supportcolor;
		}
	#else
		{
		railcolor=0xffff0000;
		spinecolor=0xff00ff00;
		rungcolor=0xffffff00;
		catwalkcolor=0xffff00ff;
		handrailcolor=0sff00ffff;
		handrungcolor=0xffffff00;
		supportcolor =0xff0000ff;
		}
	#endif


	cout<<"creating track mesh..."<<endl;

	//set specular and shininess to constants for now:
		SColor specular=SColor(255,255,255,255);
		float shininess=5.0;

	TrackMesh mesh(track,trackop,smgr,device,driver
	               ,railcolor,spinecolor,rungcolor,catwalkcolor
	               ,handrailcolor,handrungcolor,supportcolor
	               ,specular,shininess);

	////mesh.init(track,1.0);

	TrackMesh::PatternType trackstyle;
	EnterRungType:
		{
		std::string stylestr;
		cout<<"Enter track style (ladder, corkscrew, looper,beamer,lattice,rocket,rocket,wood,kamikaze): ";
		cin>>stylestr;
		if(stylestr=="corkscrew")
			trackstyle=TrackMesh::pat_corkscrew;
		else if(stylestr=="looper")
			trackstyle=TrackMesh::pat_looper;
		else if(stylestr=="beamer")
			trackstyle=TrackMesh::pat_beamer;
		else if(stylestr=="lattice")
			trackstyle=TrackMesh::pat_lattice;
		else if(stylestr=="rocket")
			trackstyle=TrackMesh::pat_rocket;
		else if(stylestr=="wood")
			trackstyle=TrackMesh::pat_wood;
		else if(stylestr=="kamikaze")
			trackstyle=TrackMesh::pat_kamikaze;
		else if(stylestr=="derby")
			trackstyle=TrackMesh::pat_derby;
		////else if(stylestr=="ladder")
		////	trackstyle=TrackMesh::pat_ladder;
		else	//default:
			trackstyle=TrackMesh::pat_ladder;
		}

	mesh.MakeTrack(trackstyle,true);			//edit this line to change track type
	cout<<"Track mesh was created"<<endl;

	ILightSceneNode *node = smgr->addLightSceneNode(0, vector3df(0,0,1000000),
									SColorf(1.0f,1.0f,1.0f,1.0f), 1.0f);
	node->setLightType(ELT_DIRECTIONAL);
	node->setRotation(vector3df(75.0,0.0,0.0));
	/*
	ILightSceneNode *backlight = smgr->addLightSceneNode(0, vector3df(0,0,1000000),
									SColorf(1.0f,0.1f,0.1f,0.1f), 1.0f);
	backlight->setLightType(ELT_DIRECTIONAL);
	backlight->setRotation(vector3df(-60.0,0.0,0.0));
	*/

	#if 1
		bool dofps=false;
		cout<<"Do you want to do a free view of the";
		cout<<" track without riding it?"<<endl;
		cin>>dofps;
		cout<<endl;
		cout<<"You entered "<<dofps<<endl;
	#else
		#define dofps 0
	#endif

	bool camchanged=false;
	ICameraSceneNode* camera;
	ICameraSceneNode* camfps;
	ICameraSceneNode* campov;
	enum CamType	{	ct_fps=0,ct_pov	};
	CamType camtype;
	campov = smgr->addCameraSceneNode();
	camfps = smgr->
		addCameraSceneNodeFPS
			( 	0,100.0f,0.05f,-1,0,0,false,0.f,false,true);
	if(dofps)
		{
		camtype=ct_fps;
		camera=camfps;
		}
	else
		{
		camtype=ct_pov;
		camera=campov;
		}
	smgr->setActiveCamera(camera);

	if(campov)
		{
		campov->setPosition(vector3df(0.f, 25.f, 0.f));
		campov->setTarget(vector3df(0.f, 0.f, 0.f));
		campov->setFarValue(20000.0f);
		campov->setNearValue(0.05f);
		}
	if(camfps)
		{
		camfps->setPosition(vector3df(0.f, 25.f, 0.f));
		camfps->setTarget(vector3df(0.f, 0.f, 0.f));
		camfps->setFarValue(20000.0f);
		camfps->setNearValue(0.05f);
		}

	#if 0
	//try loading a car model into the scene:
		IAnimatedMesh* carmesh=smgr->getMesh("models/trackedrides/Cork_Train_2/Cork_Lead_2.3ds");
		IAnimatedMeshSceneNode* carnode = smgr->addAnimatedMeshSceneNode( carmesh );
		Orientation carori=track->getbankedori(100.0);
		carnode->setPosition(carori.pos);
		matrix4 trans;
		trans[0]=carori.hdg.getrgt().X;
		trans[1]=carori.hdg.getrgt().Y;
		trans[2]=carori.hdg.getrgt().Z;
		trans[3]=0.0;
		trans[4+0]=carori.hdg.getup().X;
		trans[4+1]=carori.hdg.getup().Y;
		trans[4+2]=carori.hdg.getup().Z;
		trans[4+3]=0.0;
		trans[8+0]=carori.hdg.getfwd().X;
		trans[8+1]=carori.hdg.getfwd().Y;
		trans[8+2]=carori.hdg.getfwd().Z;
		trans[8+3]=0.0;
		trans[12+0]=0.0;
		trans[12+1]=0.0;
		trans[12+2]=0.0;
		trans[12+3]=0.0;
		vector3df carrot=trans.getRotationDegrees();
		carnode->setRotation(carrot);
	#endif

	/*
	Just a usual render loop with event handling. The custom mesh is
	a usual part of the scene graph which gets rendered by drawAll.
	*/
	f32 trackpos=0.0f;
	f32 trackpos_inc=0.01f;
	f32 tracklen=track->getTrackLen();
	f32 trainspeed=0.0f;
	f32 maxspeed=0.0f;

	bool done=false;
	bool init=false;

	Train*leadtrain;
	leadtrain=trackop->trains.at(0);

	//load a whole train
		char carModelAry[2][128]=
			{
			////"models/trackedrides/Kiddie_Coaster_Train-mopaso/Lead Truck.3ds"
			////  "models/trackedrides/Kiddie_Coaster_Train-mopaso/Kiddie Car.3ds"
			////, "models/trackedrides/Kiddie_Coaster_Train-mopaso/Kiddie Car.3ds"
			  "models/trackedrides/Corkscrew Train 3DS-TS6/Corkscrew Car 1.3ds"
			, "models/trackedrides/Corkscrew Train 3DS-TS6/Corkscrew Car 1.3ds"
			////  "models/trackedrides/Cork_Train_2/Cork_Lead_2.3ds"
			////, "models/trackedrides/Cork_Train_2/Cork_Rear_2.3ds"
			};
		deque<char*> carModelList;
		for(int i=0;i<leadtrain->cars.size();i++)
			{
			if(i==0)
				carModelList.push_back(carModelAry[0]);
			else
				carModelList.push_back(carModelAry[1]);
			}

		TrainNodeList trainnodes(leadtrain,smgr);
		trainnodes.build(carModelList);

	while(!done)
		{
		done=!device->run();

		//update train car nodes:
			trainnodes.orientToTrain();



		#if 0
		if(!device->isWindowActive())
			{
			device->sleep(100);
			continue;
			}
		#endif

		#if 1
		mesh.node->setMaterialFlag(video::EMF_WIREFRAME, false);
		if(receiver.IsKeyDown(irr::KEY_KEY_W))
			{
			////meshnode->setMaterialFlag(video::EMF_WIREFRAME, !meshnode->getMaterial(0).Wireframe);
			mesh.node->setMaterialFlag(video::EMF_WIREFRAME, true);
			}
		#endif

		if(receiver.IsKeyDown(irr::KEY_ESCAPE))
			{
			done=done||true;
			}

		UpdateTime:
			if(!init)
				lasttime=timer->getTime()-1.0;
			else
				lasttime=thistime;
			thistime=timer->getTime();
			timeElapsed=thistime-lasttime;
			timeElapsed/=1000.0f;


		ToggleLabels:
			{
			static bool waitForLRelease=false;
			if(receiver.IsKeyDown(irr::KEY_KEY_L))
				{
				if(!waitForLRelease)
					{
					tlabel.ToggleVisibility();
					waitForLRelease=true;
					}
				}
			else
				waitForLRelease=false;
			}


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


		////if(receiver.IsKeyDown(irr::KEY_KEY_S))
			{
			trackpos_inc=0;
			////'device->sleep(10);
			}

		////if(!dofps)

			{

			float raisemul=speedmul;

			static f32 headht=0.0f;		//0.75 meters off the track
			const f32 headht_from_track=0.75f;		//0.75 meters off the track
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
			if(receiver.IsKeyDown(irr::KEY_KEY_4))
				{
				timer->setSpeed(64.0f);
				}

			if(receiver.IsKeyDown(irr::KEY_KEY_C))		//change _C_amera
				{
				if(!camchanged)
					{
					if(camtype==ct_fps)
						{	camera=campov;	camtype=ct_pov;	}
					else if(camtype==ct_pov)
						{	camera=camfps;	camtype=ct_fps;	}
					camchanged=true;
					}
				}
			else
				camchanged=false;

			bool isinverted=false;
			bool islaydown=false;
			if(receiver.IsKeyDown(irr::KEY_KEY_I))
				isinverted=true;
			else if(receiver.IsKeyDown(irr::KEY_KEY_F))
				islaydown=true;

			if(campov)
				{
				SetOrientation:
					Orientation ori;
					#if 0	//manual speed control:
						ori=track->getbankedori( trackpos);
						trackpos+=trackpos_inc;
						trackpos=fmod((float)trackpos,(float)tracklen);
						if(trackpos<0.0)	trackpos+=tracklen;
					#else	//Track operator and physics:
						trackop->MoveTrain(0,timeElapsed);
						Car&car=trackop->trains.at(0)->cars.at(0);
						ori=car.ori;
						trainspeed=trackop->trains.at(0)->speed;
						maxspeed=max(trainspeed,maxspeed);
						trackpos=trackop->trains.at(0)->cars.at(0).linpos;
					#endif
				PrintOri:
					if(  (  trackpos>=track->getTrackLen()*0.995
						   /*||trackpos<=track->getTrackLen()*0.00*/)
						  &&(timer->getTime()%1==0)             )
						{
						////ori=track->getbankedori(trackpos);
						cout<<"Current Track Orientation @ trackpos=";
						cout<<trackpos<<"("<<(trackpos/track->getTrackLen()*100.0)<<"%):"<<endl;
						ori.debugprint("",0);
						}
				SetCamPOV:
					headht=isinverted?	1.0	:	(islaydown?	0.50	:	2.0	);
					ori.pos=ori.pos+headht*ori.hdg.getup();
					campov->setPosition(ori.pos);
					campov->setTarget(ori.pos+ori.hdg.getfwd());
					if(isinverted)
						ori.hdg.setup(-ori.hdg.getup());
					else if(islaydown)
						ori.hdg.setfromupfwd(ori.hdg.getfwd(),ori.hdg.getup(),true);
					campov->setTarget(ori.pos+ori.hdg.getfwd());
					campov->setUpVector( ori.hdg.getup());
					campov->setFarValue(20000.0f);
					campov->setNearValue(0.05f);
				DisplayInfo:
					static int lasttime=0;
					bool timetodisplay=(timer->getTime()-lasttime>50);
					if(timetodisplay)
						{
						lasttime=timer->getTime();
						int i=0;
						stringstream ss;
						char s[64];
						wchar_t wcs[64];
						////memset(s,0,sizeof(s));
						for(int i=0;i<msgary.size();i++)
							{
							ss.str("");
							int curtime=timer->getTime();
							int casev=0;	//casev is for a dynamic "switch/case" equivalent block
							if(i==casev++)	ss<<"elapsed time = "<<(curtime/1000/60)<<":"<<(curtime/1000%1000)<<"."<<(curtime%1000);
							else if(i==casev++)	ss<<"op mode stage = "<<int(trackop->stage);
							else if(i==casev++)	ss<<"track len (m) = "<<tracklen;
							else if(i==casev++)	ss<<"track pos (m) = "<<trackpos;
							else if(i==casev++)	ss<<"pos_inc (m) = "<<trackpos_inc;
							else if(i==casev++)	ss<<"appx cur ht (m) = "<<ori.pos.Y;
							else if(i==casev++)	ss<<"appx cur ht (ft) = "<<(ori.pos.Y*feet_per_meter);
							else if(i==casev++)	ss<<"train speed (m/s) = "<<trainspeed;
							else if(i==casev++)	ss<<"train speed (mph) = "<<(trainspeed/miles_per_hour_to_meter_per_second);
							else if(i==casev++)	ss<<"max speed (mph) = "<<(maxspeed/miles_per_hour_to_meter_per_second);
							else if((i==casev)||(i==casev+1)||(i==casev+2))
								{
								vector3df&gf=trackop->trains.at(0)->cars.at(0).gforces;
								if(i==casev+0)			ss<<"lat g's: "<<gf.X;
								else if(i==casev+1)	ss<<"vrt g's: "<<gf.Y;
								else if(i==casev+2)	ss<<"acc g's: "<<gf.Z;
								casev+=3;
								}
							else
								ss<<"___________";
							strcpy(s,ss.str().c_str());
							memset(wcs,0,sizeof(wcs));
							mbstowcs (wcs,s,strlen(s));
							msgary[i]->setText(wcs);
							}
						}
				}

			}	// end of if(!dofps)
		////else
			{
			if(  receiver.IsKeyDown(irr::KEY_UP)||receiver.IsKeyDown(irr::KEY_DOWN)
			   ||receiver.IsKeyDown(irr::KEY_RIGHT)||receiver.IsKeyDown(irr::KEY_LEFT)
			   ||receiver.IsKeyDown(irr::KEY_PRIOR)||receiver.IsKeyDown(irr::KEY_NEXT)	 )
				{
				vector3df campos=camfps->getPosition();
				vector3df camfwd,camrgt,camup;
				camfwd=camfps->getTarget()-campos;
				camup=camfps->getUpVector();
				camrgt=camup.crossProduct(camfwd);
				camfwd.normalize();
				camrgt.normalize();
				float movescale;
				if(receiver.IsKeyDown(irr::KEY_LCONTROL)||receiver.IsKeyDown(irr::KEY_RCONTROL))
					movescale=0.01;
				else if(receiver.IsKeyDown(irr::KEY_LSHIFT)||receiver.IsKeyDown(irr::KEY_RSHIFT))
					movescale=1.0;
				else
					movescale=0.1;

				if(receiver.IsKeyDown(irr::KEY_UP))
					camfps->setPosition(camfps->getPosition()+camfwd*movescale);
				if(receiver.IsKeyDown(irr::KEY_DOWN))
					camfps->setPosition(camfps->getPosition()-camfwd*movescale);
				if(receiver.IsKeyDown(irr::KEY_RIGHT))
					camfps->setPosition(camfps->getPosition()+camrgt*movescale);
				if(receiver.IsKeyDown(irr::KEY_LEFT))
					camfps->setPosition(camfps->getPosition()-camrgt*movescale);
				if(receiver.IsKeyDown(irr::KEY_PRIOR))
					camfps->setPosition(camfps->getPosition()+camup*movescale);
				if(receiver.IsKeyDown(irr::KEY_NEXT))
					camfps->setPosition(camfps->getPosition()-camup*movescale);
				}
			}	//end of else


		if(receiver.IsKeyDown(irr::KEY_KEY_T))
			{
			//top-down view:
				camfps->setPosition(vector3df(0.0f,200.0f,0.0f));
				camfps->setTarget(vector3df(0.0f,0.0f,0.0f));
				////camfps->setFarValue(20000.0f);
				////camfps->setNearValue(0.05f);
			////'device->sleep(10);
			}

		smgr->setActiveCamera(camera);

		node->getLightData().AmbientColor.set(0.0,0.0,0.0);

		driver->beginScene(true, true, SColor(0xff000000));
		smgr->drawAll();
		guienv->drawAll();
		driver->endScene();

		if(!init)
			{
			timer->setSpeed(0.0f);
			init=true;
			}


		}  //end of while(!done) loop

	device->drop();

	return 0;
	}

/*
That's it! Just compile and play around with the program.
**/

#endif








