#include "TrackFactory.h"

/**#############################################################
	TrackFactory() - public constructor for TrackFactory singleton object
##############################################################*/

TrackFactory::TrackFactory()
	{
	//ctor
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
	createTestTrack() - creates a test track.
##############################################################*/

Track*TrackFactory::createTestTrack(vector3df startpos)
	{
	Track*t=new Track();

	t->path.clear();
	////PathSpline curspline;
	DefineElements: 	//define some elements:
		PathSpline str8;
			str8.setcp(0,vector3df(0.0,0.0,7.5));
			str8.setcp(1,vector3df(0.0,0.0,10.0));
			str8.setup(0,vector3df(0.0,1.0,0.0));
			str8.setup(1,vector3df(0.0,1.0,0.0));
			str8.setup(2,vector3df(0.0,1.0,0.0));
			str8.setStartLen(2.5);
		PathSpline lcorky;
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
		PathSpline rcorky;
			rcorky=lcorky;
			rcorky.MirrorX();
		PathSpline lturn;
			f32 turn_cp_span=0.55*10.0;
			lturn.setcp(0,vector3df(-turn_cp_span,0.0,10.0));
			lturn.setcp(1,vector3df(-10.0,0.0,10.0));
			lturn.setup(0,vector3df(0.0,1.0,0.0));
			lturn.setup(1,vector3df(0.0,1.0,0.0));
			lturn.setup(2,vector3df(0.0,1.0,0.0));
			lturn.setStartLen(turn_cp_span);
		PathSpline rturn;
			rturn=lturn;
			rturn.MirrorX();
		PathSpline hillup;
			f32 hill_height=5.0f;
			hillup.setcp(0,vector3df(0.0,hill_height,6.0));
			hillup.setcp(1,vector3df(0.0,hill_height,10.0));
			hillup.setup(0,vector3df(0.0,1.0,-1.0));
			hillup.setup(1,vector3df(0.0,1.0,-1.0));
			hillup.setup(2,vector3df(0.0,1.0,0.0));
			hillup.normalize();
			hillup.setStartLen(6.0);
		PathSpline hilldn;
			hilldn.setcp(0,vector3df(0.0,-hill_height,4.0));
			hilldn.setcp(1,vector3df(0.0,-hill_height,10.0));
			hilldn.setup(0,vector3df(0.0,1.0,1.0));
			hilldn.setup(1,vector3df(0.0,1.0,1.0));
			hilldn.setup(2,vector3df(0.0,1.0,0.0));
			hilldn.normalize();
			hilldn.setStartLen(4.0);
		PathSpline rqturn;
			rqturn=rturn;
			f32 sqrt_onehalf=sqrt(0.5f);
			f32 qturn_cpspan=2.4f;
			f32 qturn_cpspan_45deg=qturn_cpspan*sqrt_onehalf;
			rqturn.setcp(0,vector3df(10.0f-10.0f*sqrt_onehalf-qturn_cpspan_45deg,0.0f,10.0f*sqrt_onehalf-qturn_cpspan_45deg));
			rqturn.setcp(1,vector3df(10.0f-10.0f*sqrt_onehalf,0.0f,10.0f*sqrt_onehalf));
			rqturn.setStartLen(qturn_cpspan);
		PathSpline lqturn;
			lqturn=rqturn;
			lqturn.MirrorX();
		PathSpline ruhalfloop;
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
		PathSpline luhalfloop;
			luhalfloop=ruhalfloop;
			luhalfloop.MirrorX();
		PathSpline rdhalfloop;
			rdhalfloop.setcp(0,vector3df(loop_wd,loop_sz,loop_base-loop_sz/2.0));
			rdhalfloop.setcp(1,vector3df(loop_wd,loop_sz,0.0-loop_sz/2.0));
			rdhalfloop.setup(0,vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(1,vector3df(0.0,0.0,-1.0));
			rdhalfloop.setup(2,vector3df(0.0,-1.0,0.0));
			rdhalfloop.normalize();
			rdhalfloop.setStartLen(loop_sz/2.0);
		PathSpline ldhalfloop;
			ldhalfloop=rdhalfloop;
			ldhalfloop.MirrorX();
		PathSpline valley30deg;
			valley30deg.setcp(0,vector3df(0.0,0.5,5.0+sqrt(3.0f)*2.5f/5.0f));
			valley30deg.setcp(1,vector3df(0.0,2.5f,5.0+sqrt(3.0f)*2.5f));
			valley30deg.setup(0,vector3df(0.0,1.0,0.0));
			valley30deg.setup(1,vector3df(0.0,2.0,-1.0));
			valley30deg.setup(2,vector3df(0.0,2.0,-1.0));
			valley30deg.normalize();
			valley30deg.setStartLen(2.5);
		PathSpline crest30deg;
			crest30deg.setcp(0,vector3df(0.0,-0.5,5.0+sqrt(3.0f)*2.5f/5.0f));
			crest30deg.setcp(1,vector3df(0.0,-2.5f,5.0+sqrt(3.0f)*2.5f));
			crest30deg.setup(0,vector3df(0.0,1.0,0.0));
			crest30deg.setup(1,vector3df(0.0,2.0,1.0));
			crest30deg.setup(2,vector3df(0.0,2.0,1.0));
			crest30deg.normalize();
			crest30deg.setStartLen(2.5);

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
			t->path.back().ScaleSpline(6.0);
			t->path.push_back(hilldn);
			t->path.back().ScaleSpline(6.0);
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

	SetTrackStart:
		t->startpos=startpos;
		t->startup.set(0.0,1.0,0.0);
		t->startfwd.set(0.0,0.0,1.0);
	InitializeTables:
		t->initTablesFromPathSpline();
	ReturnValue:
		return t;
	}
