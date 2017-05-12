/*
 *  tvox
 */

#ifndef ROTT_WIDGET_H
#define ROTT_WIDGET_H

#include <qt/Base3DView.h>
#include <ogl/DrawParticle.h>

namespace aphid {
    
class DrawGrid2;

class RotationHandle;

namespace cvx {

class Triangle;

}

namespace sdb {

template<typename T>
class VectorArray;

template<typename T>
class ValGrid;
}

template<int I>
class KdNNode;

template<typename T1, typename T2>
class KdNTree;

}

class GLWidget : public aphid::Base3DView, public aphid::DrawParticle
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

protected:
    virtual void clientInit();
    virtual void clientDraw();
	virtual void clientSelect(QMouseEvent *event);
    virtual void clientDeselect(QMouseEvent *event);
    virtual void clientMouseInput(QMouseEvent *event);
	
public slots:

signals:

private:
	
private:
typedef aphid::KdNTree<aphid::cvx::Triangle, aphid::KdNNode<4> > TreeTyp;
	TreeTyp * m_tree;
	aphid::sdb::VectorArray<aphid::cvx::Triangle > * m_triangles;
	aphid::Matrix44F m_space;
	aphid::Ray m_incident;
	aphid::RotationHandle * m_roth;

	typedef aphid::sdb::ValGrid<aphid::PosNmlCol> VGDTyp;
	VGDTyp * m_valGrd;

	aphid::DrawGrid2 * m_drdg;
};

#endif
