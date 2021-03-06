/*
 *  SplineBillboard.h
 *
 *  billboard with spline adjust width
 *
 *  Created by jian zhang on 8/9/17.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
 
#ifndef APH_SPLINE_BILLBOARD_H
#define APH_SPLINE_BILLBOARD_H

#include "BillboardMesh.h"
#include <math/SplineMap1D.h>

namespace aphid {
    
class SplineBillboard : public BillboardMesh {
     	
	SplineMap1D m_centerSpline;
	SplineMap1D m_leftSpline;
	SplineMap1D m_rightSpline;
	
public:
	SplineBillboard();
    virtual ~SplineBillboard();
	
	virtual void setBillboardSize(float w, float h, int nu, int addNv);
	
	SplineMap1D* centerSpline();
	SplineMap1D* leftSpline();
	SplineMap1D* rightSpline();
	
	void adjustCenter();
	void adjustLeft();
	void adjustRight();
	
protected:
    		
private:
    float rowCenter(int i) const;
	
};

}
#endif
