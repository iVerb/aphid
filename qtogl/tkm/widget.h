#ifndef TKM_GLWIDGET_H
#define TKM_GLWIDGET_H

#include <qt/Base3DView.h>

namespace aphid {

class Vector3F;

template<typename t>
class KMeansClustering2;

}

class GLWidget : public aphid::Base3DView
{
    Q_OBJECT

public:

    GLWidget(QWidget *parent = 0);
    ~GLWidget();
	
protected:    
    virtual void clientInit();
    virtual void clientDraw();
    virtual void clientSelect(aphid::Vector3F & origin, aphid::Vector3F & ray, aphid::Vector3F & hit);
    virtual void clientDeselect();
    virtual void clientMouseInput(aphid::Vector3F & stir);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
    
public slots:
		
private:
    
private slots:

private:
	aphid::Vector3F * m_pos;
	aphid::Vector3F * m_nml;
	aphid::KMeansClustering2<float> * m_cluster;
};

#endif
