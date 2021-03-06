#include <GeoDrawer.h>
#include <QtGui>
#include <QtOpenGL>
#include "widget.h"
#include <ogl/DrawCircle.h>
#include <ogl/RotationHandle.h>
#include <ogl/TranslationHandle.h>
#include <ogl/ScalingHandle.h>
#include <BaseCamera.h>
#include <ogl/DrawBox.h>
#include <ogl/DrawDop.h>
#include <ogl/DrawArrow.h>
#include <math/AOrientedBox.h>

using namespace aphid;

GLWidget::GLWidget(QWidget *parent)
    : Base3DView(parent)
{ 
	usePerspCamera(); 	
}

GLWidget::~GLWidget()
{}

void GLWidget::clientInit()
{
	m_space.translate(1,1,1);
	m_roth = new RotationHandle(&m_space);
	m_roth->setRadius(3.2f);
    m_tranh = new TranslationHandle(&m_space);
    m_tranh->setRadius(3.2f);
	m_scalh = new ScalingHandle(&m_space);
    m_scalh->setRadius(3.2f);
}

void GLWidget::clientDraw()
{
	getDrawer()->m_surfaceProfile.apply();
	testDops();
	
	getDrawer()->m_markerProfile.apply();

	//getDrawer()->setColor(0.f, .34f, .45f);
	DrawArrow da;
	da.drawCoordinateAt(&m_space);
	
    drawScaling();
}

void GLWidget::drawTranslate()
{
    Vector3F veye = getCamera()->eyeDirection();
	Matrix44F meye = m_space;
	meye.setFrontOrientation(veye );

	m_tranh->draw(&meye);
}

void GLWidget::drawRotate()
{
	float m[16];
	m_space.glMatrix(m);
	
	Vector3F veye = getCamera()->eyeDirection();
	Matrix44F meye = m_space;
	meye.setFrontOrientation(veye );

	m_roth->draw(&meye);
	
}

void GLWidget::drawScaling()
{
	Vector3F veye = getCamera()->eyeDirection();
	Matrix44F meye = m_space;
	meye.setFrontOrientation(veye );

	m_scalh->draw(&meye);
}

void GLWidget::clientSelect(QMouseEvent *event)
{
	//m_roth->begin(getIncidentRay() );
    //m_tranh->begin(getIncidentRay() );
	m_scalh->begin(getIncidentRay() );
	update();
}

void GLWidget::clientDeselect(QMouseEvent *event)
{
	//m_roth->end();
    //m_tranh->end();
	m_scalh->end();
	update();
}

void GLWidget::clientMouseInput(QMouseEvent *event)
{
	//m_roth->rotate(getIncidentRay() );
    //m_tranh->translate(getIncidentRay() );
	m_scalh->scale(getIncidentRay() );
	update();
}

void GLWidget::testBoxes()
{
	BoundingBox ba(-2,1,-.5,2,2,.5);
	DrawBox dba;
	dba.updatePoints(&ba);
	
	BoundingBox bb(4,-2,-1.5,8,3,3.5);
	DrawBox dbb;
	dbb.updatePoints(&bb);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	dba.drawAWireBox();
	dbb.drawAWireBox();
	
	glEnableClientState(GL_NORMAL_ARRAY);
	dbb.drawASolidBox();
	glDisableClientState(GL_NORMAL_ARRAY);
	
	glDisableClientState(GL_VERTEX_ARRAY);	
}

void GLWidget::testDops()
{
	AOrientedBox ob;
	BoundingBox box(1,1,1,16,4,3 );
	ob.caluclateOrientation(&box);
	
	float sx[4] = { -.99, -.99, -.99,-.99};
	ob.calculateCenterExtents(&box, sx);
	
	DrawDop dd;
	dd.update8DopPoints(ob);
	
#if 0
	getDrawer()->m_wireProfile.apply();
#endif
	glEnableClientState(GL_VERTEX_ARRAY);
	
#if 0
	dd.drawAWireDop();
#else
	glEnableClientState(GL_NORMAL_ARRAY);
	dd.drawASolidDop();
	glDisableClientState(GL_NORMAL_ARRAY);
#endif
	glDisableClientState(GL_VERTEX_ARRAY);	
}
	
