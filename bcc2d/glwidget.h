#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <Base3DView.h>

class BccWorld;
class BccInterface;
class FitTest;
class GLWidget : public Base3DView
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();
	
public slots:
	void importGrowMesh();
	void importCurve();
	void importPatch();
signals:

protected:
    virtual void clientInit();
    virtual void clientDraw();
	virtual void clientSelect(QMouseEvent *event);
    virtual void clientDeselect(QMouseEvent *event);
    virtual void clientMouseInput(QMouseEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	
private:

private:
	BccWorld * m_world;
	BccInterface * m_interface;

private slots:
    
};
//! [3]

#endif
