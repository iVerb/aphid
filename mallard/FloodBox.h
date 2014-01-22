/*
 *  FloodBox.h
 *  mallard
 *
 *  Created by jian zhang on 1/23/14.
 *  Copyright 2014 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include <QGroupBox>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

class QDoubleEditSlider;
class QIntEditSlider;
class FloodBox : public QGroupBox {
	Q_OBJECT

public:
	FloodBox(QWidget * parent = 0);
	
	double radius() const;
	double strength() const;
	
signals:
	void radiusChanged(double x);
	void strengthChanged(double x);
	void numSampleChanged(int x);
	void initialCurlChanged(double x);
	void floodRegionChanged(int x);
	
private slots:
	void sendRadius(double x);
	void sendStrength(double x);
	void sendNumSample(int x);
	void sendCurl(double x);
	void sendFloodRegion(int x);
	
private:
	QDoubleEditSlider * m_radiusValue;
	QIntEditSlider * m_numSampleValue;
	QDoubleEditSlider * m_curlValue;
	QDoubleEditSlider * m_strengthValue;
	QCheckBox * m_floodAreaCheck;
};