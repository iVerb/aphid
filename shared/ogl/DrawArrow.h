/*
 *  DrawArrow.h
 *  
 *
 *  Created by jian zhang on 1/1/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef APH_OGL_DRAW_ARROW_H
#define APH_OGL_DRAW_ARROW_H

namespace aphid {

class Matrix44F;

class DrawArrow {

public:
	DrawArrow();
	virtual ~DrawArrow();
	
	void drawArrowAt(const Matrix44F * mat);
	void drawCoordinateAt(const Matrix44F * mat);
	
protected:

private:
};

}
#endif