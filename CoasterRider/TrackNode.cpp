#include "TrackNode.h"
#include <vector>

TrackNode::TrackNode(
			 ISceneNode*parent,scene::ISceneManager*mgr,s32 id
			,const core::vector3df &  	position
			,const core::vector3df &  	rotation
			,const core::vector3df &  	scale
			,Track*_track
			)
	:	ISceneNode(parent,mgr,id,position,rotation,scale)
	{
	track=_track;

	this->Material.Wireframe = true;
	this->Material.Lighting = false;

	std::vector<core::vector3d<float> >p;
	core::vector3d<float> basetri[3]
		=	{{-0.5,0,0},{0.5,0,0},{0.25,-0.25,0}};
	core::vector3d<float> rottri[2];

	core::vector3d<float> A,B,C;
	float ABdot,BCdot,ABmag,BCmag;
	float cosABang,cosBCang,ABang,BCang,angtotal;

	//calculate angle of rotation
	#if 0
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
	/*
	std::cout<<"total curve angle: "<<angtotal<<std::endl;
	std::cout<<"; steps: "<<nverts<<std::endl;
	*/

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
	#endif

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

void TrackNode::OnRegisterSceneNode()
	{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
	}

void TrackNode::render()
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

const core::aabbox3d<f32>& TrackNode::getBoundingBox() const
	{
		return Box;
	}
