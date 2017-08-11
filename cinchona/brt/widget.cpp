#include <QtGui>
#include <QtOpenGL>
#include "widget.h"
#include <GeoDrawer.h>
#include <ogl/DrawCircle.h>
#include <ogl/RotationHandle.h>
#include <BaseCamera.h>
#include "../FeatherMesh.h"
#include "../FeatherDeformer.h"

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
	m_roth->setSpeed(0.07f);
	m_mesh = new FeatherMesh(20.f, 0.05f, 0.3f, 0.17f);
	m_mesh->create(20,2);
	m_deform = new FeatherDeformer(m_mesh);
	
}

void GLWidget::clientDraw()
{
	getDrawer()->m_markerProfile.apply();

	float m[16];
	m_space.glMatrix(m);
	DrawCircle dc;
	dc.draw3Circles(m);
	
	Vector3F veye = getCamera()->eyeDirection();
	Matrix44F meye = m_space;
	meye.setFrontOrientation(veye );
	meye.glMatrix(m);

	m_roth->draw(&meye);

	getDrawer()->m_surfaceProfile.apply();
	
	drawFeather();
}

void GLWidget::clientSelect(QMouseEvent *event)
{
	m_roth->begin(getIncidentRay() );
	update();
}

void GLWidget::clientDeselect(QMouseEvent *event)
{
	m_roth->end();
	update();
}

void GLWidget::clientMouseInput(QMouseEvent *event)
{
	m_roth->rotate(getIncidentRay() );
	
	update();
}
	
void GLWidget::drawFeather()
{
	const Matrix33F rot = m_space.rotation();
	m_deform->deform(rot);
	m_deform->calculateNormal();
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
    glNormalPointer(GL_FLOAT, 0, (const GLfloat*)m_deform->deformedNormals());
        glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)m_deform->deformedPoints() );
        glDrawElements(GL_TRIANGLES, m_mesh->numIndices(), GL_UNSIGNED_INT, m_mesh->indices());
        
	glNormalPointer(GL_FLOAT, 0, (const GLfloat*)m_mesh->vertexNormals());
        glVertexPointer(3, GL_FLOAT, 0, (GLfloat*)m_mesh->points() );
        glDrawElements(GL_TRIANGLES, m_mesh->numIndices(), GL_UNSIGNED_INT, m_mesh->indices());
        
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	
}
