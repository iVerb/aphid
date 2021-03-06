/*
 *  ShrubChartView.h
 *  garden
 *
 *  Created by jian zhang on 3/30/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GAR_SHRUB_CHART_VIEW_H
#define GAR_SHRUB_CHART_VIEW_H

#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QGraphicsScene;
class QGraphicsItem;
class QPixmap;
class QPoint;
QT_END_NAMESPACE

class GardenConnection;

class ShrubChartView : public QGraphicsView
{
    Q_OBJECT

public:
    ShrubChartView(QGraphicsScene * scene, QWidget * parent = 0);
	
protected:
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
	virtual void resizeEvent ( QResizeEvent * event );
	
signals:
	
private:
	void addGlyphPiece(const QPoint & pieceTypGrp, 
		const QPixmap & px,
		const QPoint & pos);
    void processSelect(const QPoint& pos);
	void processRemove(const QPoint& pos);
	void beginProcessView(QMouseEvent *event);
	void beginProcessItem(QMouseEvent *event);
	void processItem(QMouseEvent *event);
	void panView(QMouseEvent *event);
	void doMoveItem(const QPoint& mousePos);
	void doMoveConnection(const QPoint& mousePos);
	void doConnectItem(QGraphicsItem* item);
	void doRemoveConnection(QGraphicsItem* item);
	
	enum Mode {
		mNone = 0,
		mPanView = 1,
		mMoveItem = 2,
		mConnectItems = 3,
		mRemoveConnection = 4,
	}; 
	
	Mode m_mode;
	QGraphicsItem * m_selectedItem;
	GardenConnection * m_selectedConnection;
	QPoint m_lastMosePos;
/// changed after pan
	QPoint m_sceneOrigin;
	
};

#endif