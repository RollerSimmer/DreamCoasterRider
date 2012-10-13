#include "TrackPartPattern.h"

/**#################################################################################
	TrackPartPattern() - constructor for class
##################################################################################*/

TrackPartPattern::TrackPartPattern(TrackColors&_colors)
		:	colors(_colors)
	{
	vertices.clear();
	indices.clear();
	}

/**################################################################################
	MakeMeshBuffer() - make a mesh buffer
		Out:
			(return value) - a pointer to the new IMeshBuffer.
#################################################################################*/

IMeshBuffer*TrackPartPattern::MakeMeshBuffer()
	{
	SMeshBuffer*buf=new SMeshBuffer();
	if(vertices.size()>0&&indices.size()>0)
		{
		//set specular and shininess code here:
			SMaterial&mat=buf->getMaterial();
			mat.SpecularColor=colors.specular;
			mat.Shininess=colors.shininess;
		//set ambient color:
			SColor&amb=mat.AmbientColor;
			amb=colors.c1;
			amb.setRed(amb.getRed()/2);
			amb.setGreen(amb.getGreen()/2);
			amb.setBlue(amb.getBlue()/2);
		//create arrays of vertices and indices
			S3DVertex*vertbuf=new S3DVertex[vertices.size()];
			for(int i=0;i<vertices.size();i++)
				vertbuf[i]=vertices.at(i).makeS3DVertex();
			u16*idxbuf=new u16[indices.size()];
			for(int i=0;i<indices.size();i++)
				idxbuf[i]=indices.at(i);
		//now append the vertices and indices:
			buf->append(vertbuf,vertices.size(),idxbuf,indices.size());
		//delete the array buffers before returning:
			delete [] vertbuf;
			delete [] idxbuf;
		}
	FixNormals(buf);
	buf->recalculateBoundingBox();
	return buf;	//return the buffer without deleting it!
	}

/**###############################################################################
	addK() - add an amount to all k's, the track progress value in each vertex.
		IN:
			ko - the offset to add to all k's.
################################################################################*/

void TrackPartPattern::addK(float ko)
	{
	for(int i=0;i<vertices.size();i++)
		{
		f32&k=vertices.at(i).ijk.Z;
		k+=ko;
		}
	}


/**################################################################
	FixNormals - fix the normals of a mesh buffer
		IN: buf - the mesh buffer
################################################################**/

void TrackPartPattern::FixNormals(SMeshBuffer*buf)
	{
	S3DVertex*vertary=(S3DVertex*)buf->getVertices();
	u16*idxary=buf->getIndices();
	if(buf->getVertexType()==EVT_STANDARD)
		{
		MakeNormalsZero:
			for(int i=0;i<buf->getVertexCount();i++)
				vertary[i].Normal.set(0.0,0.0,0.0);
		CalcCrossProducts:
			for(int j=0;j<buf->getIndexCount();j+=3)
				{
				core::vector3df a,b,c,ba,bc,norm;
				a=vertary[idxary[j]].Pos;
				b=vertary[idxary[j+1]].Pos;
				c=vertary[idxary[j+2]].Pos;
				ba=a-b;
				bc=c-b;
				////norm=ba.crossProduct(bc);
				norm=bc.crossProduct(ba);
				norm.normalize();
				for(int k=1;k<2;k++)
					{
					vertary[idxary[j+k]].Normal+=norm;
					}

				}
		NormalizeNormals:
			for(int i=0;i<buf->getVertexCount();i++)
				vertary[i].Normal.normalize();
		}
	else
		////cout<<"What?"<<endl
		;
	}

