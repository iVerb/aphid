/*
 *  Plot1DWidget.h
 *  
 *  plot y = f(x)
 *  Created by jian zhang on 9/9/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef APHID_PLOT_1D_WIDGET_H
#define APHID_PLOT_1D_WIDGET_H

#include <qt/BaseImageWidget.h>
#include <math/plots.h>

namespace aphid {

class Plot1DWidget : public BaseImageWidget {

	Q_OBJECT
	
public:
	Plot1DWidget(QWidget *parent = 0);
	virtual ~Plot1DWidget();
	
/// horizontal and vertical bound 
/// low high number of intervals
	void setBound(const float & hlow, const float & hhigh, 
					const int & hnintv,
					const float & vlow, const float & vhigh, 
					const int & vnintv);
					
	void addVectorPlot(UniformPlot1D * p);
	
public slots:
   	
protected:
	virtual void clientDraw(QPainter * pr);
	
/// left-up
	QPoint luCorner() const;
/// right-bottom
	QPoint rbCorner() const;
/// from draw coordinate
	Vector2F toRealSpace(const int & x,
	                    const int & y) const;
/// remap input [0,1] to bound
	float xToBound(const float & x) const;
	
private:
	void drawCoordsys(QPainter * pr) const;
	void drawHorizontalInterval(QPainter * pr) const;
	void drawVerticalInterval(QPainter * pr) const;
	void drawPlot(const UniformPlot1D * plt, QPainter * pr);
	void drawLine(const UniformPlot1D * plt, QPainter * pr);
	void drawMark(const UniformPlot1D * plt, QPainter * pr);

private:
	Vector2F m_hBound;
	Vector2F m_vBound;
	Int2 m_niterval;
	
	std::vector<UniformPlot1D *> m_curves;
	
};

}
#endif
