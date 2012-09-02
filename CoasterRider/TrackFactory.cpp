#include "TrackFactory.h"

#include "TrackFileParser.h"

/**#############################################################
	TrackFactory() - public constructor for TrackFactory singleton object
##############################################################*/

TrackFactory::TrackFactory()
	{
	//ctor
	DefineElements();
	}

/**#############################################################
	~TrackFactory() - public destructor for TrackFactory singleton object
##############################################################*/

TrackFactory::~TrackFactory()
	{
	//dtor
	}

/**#############################################################
	getinstance() - gets the singleton instance of TrackFactory
##############################################################*/

TrackFactory*TrackFactory::getinstance()
	{
	static TrackFactory*instance=0;
	if(instance==0)
		instance=new TrackFactory();
	return instance;
	}

/**#############################################################
	DefineElements() - Defines elements
##############################################################*/


void TrackFactory::DefineElements()
	{
	DefineElements: 	//define some elements:
		define_str8:
			str8.setcp(0,vector3df(0.0,0.0,7.5));
			str8.setcp(1,vector3df(0.0,0.0,10.0));
			str8.setup(0,vector3df(0.0,1.0,0.0));
			str8.setup(1,vector3df(0.0,1.0,0.0));
			str8.setup(2,vector3df(0.0,1.0,0.0));
			str8.setStartLen(2.5);
		define_lcorky:
			#if 1
				vector3df corkcp[2],corkline[2],corkup[2];
				float corkcp_span=10.0;
				corkcp[0].set(0.0,0.0,corkcp_span);
				corkcp[1].set(corkcp_span-10.0,10.0,10.0);
				corkline[0].set(-5.0,5.0,5.0);
				corkline[1].set(-5.0,5.0,5.0);
				for(int i=0;i<2;i++)	corkup[i]=corkline[i]-corkcp[i];
				lcorky.setcp(0,corkcp[1]);
				lcorky.setcp(1,vector3df(-10.0,10.0,10.0));
				lcorky.setup(0,corkup[0]);
				lcorky.setup(1,corkup[1]);
				lcorky.setup(2,vector3df(0.0,-1.0,0.0));
				lcorky.normalize();
				lcorky.setStartLen(8.0);
				////lcorky.ScaleSpline(1.0f);
			#else
				vector3df corkcp[2],corkline[2],corkup[2];
				float corkcp_span=10.0;
				corkcp[0].set(0.0,10.0,corkcp_span);
				corkcp[1].set(-10.0+corkcp_span,10.0,10.0);
				corkline[0].set(-5.0,5.0,5.0);
				corkline[1].set(-5.0,5.0,5.0);
				for(int i=0;i<2;i++)	corkup[i]=corkline[i]-corkcp[i];
				lcorky.setcp(0,corkcp[1]);
				lcorky.setcp(1,vector3df(-10.0,10.0,10.0));
				lcorky.setup(0,corkup[0]);
				lcorky.setup(1,corkup[1]);
				lcorky.setup(2,vector3df(0.0,-1.0,0.0));
				lcorky.normalize();
				lcorky.setStartLen(corkcp_span);
				////lcorky.ScaleSpline(1.0f);
			#endif
		define_rcorky:
			rcorky=lcorky;
			rcorky.MirrorX();
		define_lturn:
			f32 turn_cp_span=0.55*10.0;
			lturn.setcp(0,vector3df(-10.0+turn_cp_span,0.0,10.0));
			lturn.setcp(1,vector3df(-10.0,0.0,10.0));
			lturn.setup(0,vector3df(0.0,1.0,0.0));
			lturn.setup(1,vector3df(0.0,1.0,0.0));
			lturn.setup(2,vector3df(0.0,1.0,0.0));
			lturn.setStartLen(turn_cp_span);
		define_rturn:
			rturn=lturn;
			rturn.MirrorX();
		define_hillup:
			f32 hill_height=5.0f;
			hillup.setcp(0,vector3df(0.0,hill_height,6.0));
			hillup.setcp(1,vector3df(0.0,hill_height,10.0));
			hillup.setup(0,vector3df(0.0,1.0,-1.0));
			hillup.setup(1,vector3df(0.0,1.0,-1.0));
			hillup.setup(2,vector3df(0.0,1.0,0.0));
			hillup.normalize();
			hillup.setStartLen(6.0);
		define_hilldn:
			hilldn.setcp(0,vector3df(0.0,-hill_height,4.0));
			hilldn.setcp(1,vector3df(0.0,-hill_height,10.0));
			hilldn.setup(0,vector3df(0.0,1.0,1.0));
			hilldn.setup(1,vector3df(0.0,1.0,1.0));
			hilldn.setup(2,vector3df(0.0,1.0,0.0));
			hilldn.normalize();
			hilldn.setStartLen(4.0);
		define_rqturn:
			rqturn=rturn;
			f32 sqrt_onehalf=sqrt(0.5f);
			f32 qturn_cpspan=2.4f;
			f32 qturn_cpspan_45deg=qturn_cpspan*sqrt_onehalf;
			rqturn.setcp(0,vector3df(10.0f-10.0f*sqrt_onehalf-qturn_cpspan_45deg,0.0f,10.0f*sqrt_onehalf-qturn_cpspan_45deg));
			rqturn.setcp(1,vector3df(10.0f-10.0f*sqrt_onehalf,0.0f,10.0f*sqrt_onehalf));
			rqturn.setStartLen(qturn_cpspan);
		define_lqturn:
			lqturn=rqturn;
			lqturn.MirrorX();
		define_ruhalfloop:
			float loop_sz=30.0;
			float loop_base=loop_sz;
			float loop_wd=loop_sz/12.0;

			ruhalfloop.setcp(0,vector3df(loop_wd,loop_sz,loop_sz));
			ruhalfloop.setcp(1,vector3df(loop_wd,loop_sz,loop_sz/2.0));
			ruhalfloop.setup(0,vector3df(0.0,0.0,-1.0));
			ruhalfloop.setup(1,vector3df(0.0,0.0,-1.0));
			ruhalfloop.setup(2,vector3df(0.0,-1.0,0.0));
			ruhalfloop.normalize();
			ruhalfloop.setStartLen(loop_base);
		define_luhalfloop:
			luhalfloop=ruhalfloop;
			luhalfloop.MirrorX();
		define_rdhalfloop:
			rdhalfloop.setcp(0,vector3df(loop_wd,loop_sz,loop_base-loop_sz/2.0));
			rdhalfloop.setcp(1,vector3df(loop_wd,loop_sz,0.0-loop_sz/2.0));
			rdhalfloop.setup(0,vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(1,vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(2,vector3df(0.0,-1.0,0.0));
			rdhalfloop.normalize();
			rdhalfloop.setStartLen(loop_sz/2.0);
		define_ldhalfloop:
			ldhalfloop=rdhalfloop;
			ldhalfloop.MirrorX();
		define_valley30deg:
			valley30deg.setcp(0,vector3df(0.0,1.25f,5.0*(1.0+sqrt(3.0)/4.0)));
			valley30deg.setcp(1,vector3df(0.0,2.5f,5.0*(1.0+sqrt(3.0)/2.0)));
			valley30deg.setup(0,vector3df(0.0,1.0,0.0));
			valley30deg.setup(1,vector3df(0.0,2.0,-1.0));
			valley30deg.setup(2,vector3df(0.0,2.0,-1.0));
			valley30deg.normalize();
			valley30deg.setStartLen(2.5);
		define_crest30deg:
			crest30deg=valley30deg;
			crest30deg.FlipY();
		define_valley90deg:
			valley90deg.setcp(0,vector3df(0.0,10.0-turn_cp_span,10.0));
			valley90deg.setcp(1,vector3df(0.0,10.0,10.0));
			valley90deg.setup(0,vector3df(0.0,1.0,-1.0));
			valley90deg.setup(1,vector3df(0.0,1.0,-1.0));
			valley90deg.setup(2,vector3df(0.0,0.0,-1.0));
			valley90deg.normalize();
			valley90deg.setStartLen(turn_cp_span);
		define_crest90deg:
			crest90deg=valley90deg;
			crest90deg.FlipY();
		define_valley45deg:
			const float sqrthalf=sqrt(1.0/2.0);
			valley45deg.setcp(0,vector3df(0.0,10.0*sqrthalf/2.0,10.0+10.0*sqrthalf/2.0));
			valley45deg.setcp(1,vector3df(0.0,10.0*sqrthalf,10.0+10.0*sqrthalf));
			valley45deg.setup(0,vector3df(0.0,2.0,-1.0));
			valley45deg.setup(1,vector3df(0.0,2.0,-1.0));
			valley45deg.setup(2,vector3df(0.0,1.0,-1.0));
			valley45deg.normalize();
			valley45deg.setStartLen(5.0);
		define_crest45deg:
			crest45deg=valley45deg;
			crest45deg.FlipY();
		define_rqroll:
			rqroll.setcp(0,vector3df(0.0,0.0,6.67));
			rqroll.setcp(1,vector3df(0.0,0.0,10.0));
			rqroll.setup(0,vector3df(-1.0,3.0,0.0));
			rqroll.setup(1,vector3df(-3.0,1.0,0.0));
			rqroll.setup(2,vector3df(-1.0,0.0,0.0));
			rqroll.normalize();
			rqroll.setStartLen(3.33);
		define_lqroll:
			lqroll=rqroll;
			lqroll.MirrorX();
	}

/**#############################################################
	TrackStart() - set the default track start values:
		In: t - the pointer to the Track
			 ori - the start orientation of the track.
##############################################################*/

void TrackFactory::SetTrackStart(Track*t,Orientation startori)
	{
	t->startpos=startori.pos;
	t->startup.set(startori.hdg.getup());
	t->startfwd.set(startori.hdg.getfwd());
	}

/**#############################################################
	createEmptyTrack() - set the default track start values:
		In: startori - the track's starting Orientation.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createEmptyTrack(Orientation startori)
	{
	Track*t=new Track();
	InitTrack:
		t->path.clear();
		SetTrackStart(t,startori);
		InitTables(t);
	ReturnTrack:
		return t;
	}

/**#############################################################
	createTestTrack() - creates a test track.
		In: startori - the track's starting Orientation.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createTestTrack(Orientation startori)
	{
	Track*t=createEmptyTrack(startori);

	AddElements:
		#if 1 //full
			int ei=0;	//element index
			t->path.push_back(str8);
			t->path.back().ScaleSpline(3.0);	//a straight track 5x longer than normal
			t->path.push_back(valley30deg);
			t->path.push_back(str8);
			t->path.back().ScaleSpline(10.0);	//a straight track 10x longer than normal
			t->path.push_back(crest30deg);
			////t->path.back().ScaleSpline(1.0);
			t->path.push_back(hilldn);
			t->path.back().ScaleSpline(10.0);	//a hill 10x longer than normal

			float cobrascale=1.25;
			t->path.push_back(ruhalfloop);
			t->path.back().ScaleSpline(cobrascale);
			t->path.push_back(lcorky);
			t->path.back().ScaleSpline(cobrascale);
			t->path.push_back(rcorky);
			t->path.back().ScaleSpline(cobrascale);
			t->path.push_back(ldhalfloop);
			t->path.back().ScaleSpline(cobrascale);
			t->path.push_back(hillup);
			t->path.back().ScaleSpline(5.0);
			for(int i=0;i<4;i++)
				{
				t->path.push_back(rqroll);
				t->path.back().ScaleSpline(2.0);
				}
			t->path.push_back(hilldn);
			t->path.back().ScaleSpline(5.0);
			t->path.push_back(hillup);
			t->path.back().ScaleSpline(5.0);
			t->path.push_back(rturn);
			t->path.back().ScaleSpline(2.0);
			t->path.push_back(hilldn);
			t->path.back().ScaleSpline(5.0);
			t->path.push_back(ruhalfloop);
			t->path.back().ScaleSpline(0.7);
			t->path.push_back(rdhalfloop);
			t->path.back().ScaleSpline(0.7);
			t->path.push_back(luhalfloop);
			t->path.back().ScaleSpline(0.7);
			t->path.push_back(ldhalfloop);
			t->path.back().ScaleSpline(0.7);
			t->path.push_back(hillup);
			t->path.back().ScaleSpline(3.5);
			t->path.push_back(rturn);
			t->path.back().ScaleSpline(1.5);
			t->path.push_back(rqturn);
			t->path.back().ScaleSpline(1.0);
			t->path.push_back(hilldn);
			t->path.back().ScaleSpline(3.5);
			for(int i=0;i<4;i++)
				{
				t->path.push_back(rcorky);
				t->path.back().ScaleSpline(1.5);
				}
			t->path.push_back(rqturn);
			t->path.back().ScaleSpline(1.0);
			t->path.push_back(rturn);
			t->path.back().ScaleSpline(2.0);
			for(int i=0;i<2;i++)
				{
				t->path.push_back(valley30deg);
				t->path.back().ScaleSpline(2.0);
				t->path.push_back(crest30deg);
				t->path.back().ScaleSpline(2.0);
				t->path.push_back(crest30deg);
				t->path.back().ScaleSpline(2.0);
				t->path.push_back(valley30deg);
				t->path.back().ScaleSpline(2.0);
				}
			t->path.push_back(valley30deg);
			t->path.back().ScaleSpline(1.5);
			t->path.push_back(crest30deg);
			t->path.back().ScaleSpline(1.5);
			for(int i=0;i<2;i++)
				{
				t->path.push_back(lturn);
				t->path.back().ScaleSpline(2.05);
				}
			t->path.push_back(str8);
			t->path.back().ScaleSpline(5.0);
			float scale30deg=1.44;
			t->path.push_back(crest30deg);
			t->path.back().ScaleSpline(scale30deg);
			t->path.push_back(valley30deg);
			t->path.back().ScaleSpline(scale30deg);
			t->path.push_back(str8);
			t->path.back().ScaleSpline(3.0);
			t->fullcircuit=true;
		#endif

		#if 0	//length test
			t->path.push_back(str8);
			#if 0
			t->path.push_back(rturn);
			t->path.push_back(lturn);
			t->path.push_back(rcorky);
			t->path.push_back(lcorky);
			#endif
			t->fullcircuit=false;
			////t->path.back().ScaleSpline(1.0);
		#endif


		#if 0 //circuit test
			t->path.push_back(str8);
			t->path.back().ScaleSpline(3.0);
			t->path.push_back(lturn);
			t->path.push_back(lturn);
			t->path.push_back(str8);
			t->path.back().ScaleSpline(6.0);
			t->path.push_back(valley30deg);
			t->path.push_back(crest30deg);
			#if 0
			t->path.push_back(lturn);
			t->path.back().ScaleSpline(1.1);
			t->path.push_back(lturn);
			t->path.back().ScaleSpline(1.1);
			t->path.push_back(str8);
			t->path.back().ScaleSpline(1.0);
			#endif
			t->fullcircuit=true;
		#endif


		#if 0	//loops
			int n;
			t->path.push_back(str8);
			t->path.back().ScaleSpline(10.0);
			t->path.push_back(ruhalfloop);
			t->path.back().ScaleSpline(1.0);
			t->path.push_back(rdhalfloop);
			t->path.back().ScaleSpline(1.0);
			t->path.push_back(ruhalfloop);
			t->path.back().ScaleSpline(1.0);
			t->path.push_back(rdhalfloop);
			t->path.back().ScaleSpline(1.0);
			for(int i=0;i<2;i++)
				{	t->path.push_back(valley30deg);
					t->path.back().ScaleSpline(2.0);	}
			for(int i=0;i<2;i++)
				{	t->path.push_back(rturn);
					t->path.back().ScaleSpline(2.0);	}
			for(int i=0;i<2;i++)
				{	t->path.push_back(valley30deg);
					t->path.back().ScaleSpline(2.0);	}
			t->path.push_back(str8);
			t->path.back().ScaleSpline(15.0);
			t->fullcircuit=true;
		#endif

		#if 0 //corks and rolls
			t->path.push_back(str8*2.0);
			t->path.push_back(lcorky*2.0);
			t->path.push_back(lcorky*2.0);
			t->path.push_back(str8*2.0);
			t->path.push_back(lqroll*2.0);
			t->path.push_back(lqroll*2.0);
			t->path.push_back(lqroll*2.0);
			t->path.push_back(lqroll*2.0);
			t->fullcircuit=false;
		#endif

		#if 0 //bunny hill
			t->path.push_back(valley30deg);
			t->path.push_back(crest30deg);
			t->path.push_back(crest30deg);
			t->path.push_back(valley30deg);
		#endif

		#if 0	//cobra roll
			int n;
			t->path.push_back(ruhalfloop);
			t->path.push_back(lcorky);
			t->path.push_back(rcorky);
			t->path.push_back(ldhalfloop);
			t->path.push_back(lturn);
			t->path.push_back(lturn);
		#endif

		#if 0		//small track with hills and corkscrews
			int n;
			#if 1
			n=5;
			for(int i=0;i<n;i++)
				{	t->path.push_back(hillup);	t->path.push_back(hilldn);	}
			t->path.push_back(hillup);
			t->path.push_back(lturn);
			t->path.push_back(lqturn);
			t->path.push_back(hilldn);
			#endif
			n=5*2;
			for(int i=0;i<n;i++)	t->path.push_back(lcorky);
			#if 1
			t->path.push_back(lqturn);
			t->path.push_back(lqturn);
			t->path.push_back(str8);
			t->path.push_back(lqturn);
			t->path.push_back(lturn);
			n=5;
			for(int i=0;i<n;i++)	t->path.push_back(str8);
			//mirror entire track:
				////for(int i=0;i<t->path.size();i++)	path[i].MirrorX();
			#endif
		#endif

		#if 0
			n=4;
			for(int i=0;i<n;i++)		t->path.push_back(str8);
			n=3;
			for(int i=0;i<n;i++)		t->path.push_back(lturn);
			n=2;
			for(int i=0;i<n;i++)		t->path.push_back(rcorky);
			n=3;
			for(int i=0;i<n;i++)		t->path.push_back(rturn);
		#endif

	InitTables(t);


	ReturnTrack:
		return t;
	}

/**#############################################################
	createCorkscrewTrack() - creates a corkscrew track.
		In: startori - the track's starting Orientation.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createCorkscrewTrack(Orientation startori,float trackscale)
	{

	Track*t=createEmptyTrack(startori);

	#if 1	//corkscrew test
		t->path.push_back(valley30deg);
		t->path.push_back(str8);
		t->path.back().ScaleSpline(4.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(crest30deg);
		t->path.push_back(crest30deg);
		t->path.push_back(valley30deg);
		t->path.push_back(rturn);
		t->path.back().ScaleSpline(1.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(rturn);
		t->path.back().ScaleSpline(1.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(hilldn);
		t->path.back().ScaleSpline(4.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(str8);
		t->path.back().ScaleSpline(1.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(valley30deg);
		t->path.back().ScaleSpline(1.5);
			t->path.back().ScaleSpline(trackscale);
		t->path.push_back(str8);
		t->path.back().ScaleSpline(1.5);
			t->path.back().ScaleSpline(trackscale);
		t->path.push_back(crest30deg);
		t->path.back().ScaleSpline(1.5);
			t->path.back().ScaleSpline(trackscale);
		t->path.push_back(rturn);
		t->path.back().ScaleSpline(1.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(rqturn);
		t->path.back().ScaleSpline(1.6);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(hilldn);
		t->path.back().ScaleSpline(2.0);
			t->path.back().ScaleSpline(trackscale);
		for(int i=0;i<4;i++)
			{
			t->path.push_back(rcorky);
			t->path.back().ScaleSpline(1.25);
			t->path.back().ScaleSpline(trackscale);
			}
		t->path.push_back(rqturn);
		t->path.back().ScaleSpline(1.5);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(rturn);
		t->path.back().ScaleSpline(2.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(rturn);
		t->path.back().ScaleSpline(2.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(str8);
		t->path.back().ScaleSpline(4.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(crest30deg);
		t->path.back().ScaleSpline(1.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(valley30deg);
		t->path.back().ScaleSpline(1.0);
		t->path.back().ScaleSpline(trackscale);
		t->path.push_back(str8);
		t->path.back().ScaleSpline(2.0);
		t->path.back().ScaleSpline(trackscale);

		for(int i=0;i<2;i++)
			{
			t->path.push_back(rturn);
			t->path.back().ScaleSpline(1.25);
			t->path.back().ScaleSpline(trackscale);
			}
		t->path.push_back(str8);
		t->path.back().ScaleSpline(5.0);
		t->path.back().ScaleSpline(trackscale);
		t->fullcircuit=true;
		////t->path.back().ScaleSpline(1.0);
	#endif

	InitTables(t);

	ReturnTrack:
		return t;
	}

/**#############################################################
	createMegaCoaster() - creates a shuttle track.
		In: startori - the track's starting Orientation.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createMegaCoasterTrack(Orientation startori,float trackscale)
	{

	Track*t=createEmptyTrack(startori);

	AssembleElements:
		t->path.push_back(valley90deg*1.0*trackscale);
		t->path.push_back(crest90deg*1.0*trackscale);
		////for(int i=0;i<4;i++)
		////	t->path.push_back(lqroll*1.0*trackscale);
		t->path.push_back(str8*5.0*trackscale);
		for(int i=0;i<12;i++)
			t->path.push_back(rqroll*1.0*trackscale);
		#if 0
		LiftHill:
			t->path.push_back(valley30deg*1.0*trackscale);
			t->path.push_back(str8*12.5*trackscale);
		FirstDrop:
			t->path.push_back(crest90deg*1.0*trackscale);
			t->path.push_back(crest30deg*1.0*trackscale);
			t->path.push_back(str8*3.0*trackscale);
			t->path.push_back(valley90deg*5.0*trackscale);
			t->path.push_back(hillup*7.0*trackscale);
			t->path.push_back(hilldn*7.0*trackscale);
			t->path.push_back(str8*10.0*trackscale);
		#endif
	t->fullcircuit=false;

	InitTables(t);

	ReturnTrack:
		return t;
	}

/**#############################################################
	createTopHatCoaster() - creates a launced top hat coaster
		In: startori - the track's starting Orientation.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createTopHatCoasterTrack(Orientation startori,float trackscale)
	{

	Track*t=createEmptyTrack(startori);

	AssembleElements:
		t->path.push_back(str8*12.0*trackscale);
		t->path.push_back(valley90deg*3.0*trackscale);
		t->path.push_back(lqroll*2.0);
		t->path.push_back(valley90deg*1.5*trackscale);
		t->path.push_back(valley90deg*1.5*trackscale);
		t->path.push_back(rqroll*1.0);
		t->path.push_back(rqroll*1.0);
		t->path.push_back(valley90deg*3.0*trackscale);
		t->path.push_back(valley45deg*2.0*trackscale);
		t->path.push_back(rturn*2.5f*trackscale);
		t->path.push_back(rturn*2.0f*trackscale);
		t->path.push_back(valley45deg*2.0*trackscale);
		t->path.push_back(rqturn*3.0*trackscale);
		t->path.push_back(str8*2.0*trackscale);
		t->path.push_back(valley90deg*2.5*trackscale);
		t->path.push_back(str8*8.0*trackscale);
		////t->path.push_back(valley45deg*2.0*trackscale);
		////t->path.push_back(rturn*2.5*trackscale);
		////t->path.push_back(rturn*2.5*trackscale);
		////t->path.push_back(valley45deg*2.0*trackscale);
		////t->path.push_back(lqturn*1.5*trackscale);
		////t->path.push_back(valley90deg*2.5*trackscale);

	t->fullcircuit=false;

	InitTables(t);

	ReturnTrack:
		return t;
	}


/**#############################################################
	createTrackFromFile() - creates a track from a file
		In: the name of the track XML file.
		Out: (return value) - the new track's pointer.
##############################################################*/

Track*TrackFactory::createTrackFromFile(char*file)
	{
	Track*t;
	#if 1
	t=TrackFileParser::getinstance()->readtrack(file);
	#endif
	InitTables(t);
	return t;
	}
