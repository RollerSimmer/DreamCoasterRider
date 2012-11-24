#include "TrackPartPatternFactory.h"

/**############################################################################
	trimake() - make a triangle face
		IN:
			pat - the pattern to add to.
			idx_base - the base vertex index.  index offsets are added to this value.
			a - the first point index offset
			b - the secont point index offset
			c - the third point index offset
############################################################################**/

void TrackPartPatternFactory::trimake( TrackPartPattern*pat,int idx_base
						  						  ,int a,int b,int c)
	{
	pat->indices.push_back(idx_base+a);
	pat->indices.push_back(idx_base+b);
	pat->indices.push_back(idx_base+c);
	}

/**############################################################################
	quadmake() - make a quad face
		IN:
			pat - the pattern to add to.
			idx_base - the base vertex index.  index offsets are added to this value.
			a - the first point index offset
			b - the secont point index offset
			c - the third point index offset
			d - the fourth point index offset
		NOTE:
			the path goes clockwise starting at a and going through b,c, and d before
			making its way back to a.
############################################################################**/

void TrackPartPatternFactory::quadmake( TrackPartPattern*pat,int idx_base
													,int a,int b,int c,int d)
	{
	trimake(pat,idx_base,a,b,c);
	trimake(pat,idx_base,d,a,c);
	}


/**############################################################################
	edgemake() - make a edge face.
		IN:
			pat - the pattern to add to.
			idx_base - the base vertex index.  index offsets are added to this value.
			fo - the front offset
			bo - the back offset
			a - the first edge point
			b - the second edge point
		NOTE:
			>	the path goes clockwise starting at a and going through b,c, and d before
				making its way back to a.
			>	this function assumes that a front and back set of points exists,
			   and that that there is an equal amount points on each set, as well
			   as points in the same order.  Designed especially for edges along
			   extruded faces.
############################################################################**/

void TrackPartPatternFactory::edgemake(TrackPartPattern*pat,int idx_base
													,int fo,int bo,int a,int b)
   {
   quadmake(pat,idx_base,a+fo,b+fo,b+bo,a+bo);
   }


