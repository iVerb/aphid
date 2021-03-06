/*
 *  AttribWidget.cpp
 *  
 *
 *  Created by jian zhang on 8/18/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include <QtGui>
#include "AttribWidget.h"
#include "ShrubScene.h"
#include "graphchart/GardenGlyph.h"
#include "gar_common.h"
#include "data/ground.h"
#include "data/grass.h"
#include "data/file.h"
#include "data/billboard.h"
#include "data/variation.h"
#include "data/stem.h"
#include "data/twig.h"
#include "data/branch.h"
#include "attr/PieceAttrib.h"
#include <qt/QDoubleEditSlider.h>
#include <qt/QStringEditField.h>
#include <qt/SplineEditGroup.h>
#include <qt/QEnumCombo.h>
#include <qt/IntEditGroup.h>
#include <qt/DoubleEditGroup.h>
#include <qt/IconButtonGroup.h>

using namespace aphid;

AttribWidget::AttribWidget(ShrubScene* scene, QWidget *parent) : QWidget(parent)
{
	m_selectedGlyph = NULL;
	m_scene = scene;
	
	mainLayout = new QVBoxLayout;
	mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2,2,2,2);
	setLayout(mainLayout);
	
	m_lastStretch = 0;
}

void AttribWidget::lsAttribs(GardenGlyph* g)
{
	lsAdded(g);
    
	const int n = m_collWigs.count();
    for (int i = 0; i < n; ++i) {
        mainLayout->addWidget(m_collWigs[i]);
    }

	m_lastStretch = new QSpacerItem(8,8, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	mainLayout->addItem(m_lastStretch);
}

void AttribWidget::clearAttribs()
{
	foreach (QWidget *widget, m_collWigs)
        mainLayout->removeWidget(widget);

	while (!m_collWigs.isEmpty())
		delete m_collWigs.dequeue();
		
	if(m_lastStretch) {
		mainLayout->removeItem(m_lastStretch);
		delete m_lastStretch;
		m_lastStretch = 0;
	}
}

void AttribWidget::lsAdded(GardenGlyph* g)
{
	PieceAttrib* att = g->attrib();
	const int n = att->numAttribs();
	for(int i=0;i<n;++i) {
		gar::Attrib* ati = att->getAttrib(i);
		lsAttr(ati);
	}
}

void AttribWidget::lsAttr(gar::Attrib* attr)
{
	const gar::AttribType atyp = attr->attrType();	
/// skip node name
	if(attr->attrName() == gar::nmNodeName)
		return;
			
	QWidget* wig = NULL;
	switch (atyp) {
		case gar::tFloat :
			wig = shoFltAttr(attr);
		break;
		case gar::tInt :
			wig = shoIntAttr(attr);
		break;
		case gar::tString :
			wig = shoStrAttr(attr);
		break;
		case gar::tSpline :
			wig = shoSplineAttr(attr);
		break;
		case gar::tEnum :
			wig = shoEnumAttr(attr);
		break;
		case gar::tVec2 :
			wig = shoVec2Attr(attr);
		break;
		case gar::tInt2 :
			wig = shoInt2Attr(attr);
		break;
		case gar::tAction :
			wig = shoActionAttr(attr);
		break;
		default:
			wig = new QLabel(tr(attr->attrNameStr().c_str() ) );
	}
	
	m_collWigs.enqueue(wig);
}

QWidget* AttribWidget::shoFltAttr(gar::Attrib* attr)
{
	QDoubleEditSlider* wig = new QDoubleEditSlider(tr(attr->attrNameStr().c_str() ) );
	float val, val0, val1;
	attr->getValue(val);
	attr->getMin(val0);
	attr->getMax(val1);
	wig->setLimit(val0, val1);
	wig->setValue(val);
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, double>)),
            this, SLOT(recvDoubleValue(QPair<int, double>)));
			
	return wig;
}

QWidget* AttribWidget::shoVec2Attr(gar::Attrib* attr)
{
	DoubleEditGroup* wig = new DoubleEditGroup(tr(attr->attrNameStr().c_str() ), 2 );
	float val[2];
	attr->getValue2(val);
	
	wig->setValues(val);
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, QVector<double> >)),
            this, SLOT(recvVec2Value(QPair<int, QVector<double> >)));
			
	return wig;
}

QWidget* AttribWidget::shoInt2Attr(gar::Attrib* attr)
{
	IntEditGroup* wig = new IntEditGroup(tr(attr->attrNameStr().c_str() ), 2 );
	int val[2];
	attr->getValue2(val);
	
	wig->setValues(val);
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, QVector<int> >)),
            this, SLOT(recvInt2Value(QPair<int, QVector<int> >)));
			
	return wig;
}

QWidget* AttribWidget::shoIntAttr(gar::Attrib* attr)
{
	IntEditGroup* wig = new IntEditGroup(tr(attr->attrNameStr().c_str() ), 1 );
	int val, val0, val1;
	attr->getValue(val);
	attr->getMin(val0);
	attr->getMax(val1);
	wig->setLimit(val0, val1);
	wig->setValue0(val);
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, QVector<int> >)),
            this, SLOT(recvIntValue(QPair<int, QVector<int> >)));
			
	return wig;
}

QWidget* AttribWidget::shoStrAttr(gar::Attrib* attr)
{
	gar::StringAttrib* sattr = static_cast<gar::StringAttrib*> (attr);
	std::string sval;
	sattr->getValue(sval);
	QStringEditField* wig = new QStringEditField(tr(attr->attrNameStr().c_str() ) );
	wig->setValue(tr(sval.c_str() ) );
	if(sattr->isFileName() ) {
	    wig->addButton(":/icons/document_open.png");
	    wig->setSelectFileFilter("*.hes;;*.m");
	}
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, QString>)),
            this, SLOT(recvStringValue(QPair<int, QString>)));
    
	return wig;
}

QWidget* AttribWidget::shoSplineAttr(gar::Attrib* attr)
{
	gar::SplineAttrib* sattr = static_cast<gar::SplineAttrib*> (attr);
	
	SplineEditGroup* wig = new SplineEditGroup(tr(attr->attrNameStr().c_str() ) );
	wig->setNameId(attr->attrName() );
	
	float tmp[2];
	sattr->getSplineValue(tmp);
	wig->setSplineValue(tmp);
	sattr->getSplineCv0(tmp);
	wig->setSplineCv0(tmp);
	sattr->getSplineCv1(tmp);
	wig->setSplineCv1(tmp);
	
	connect(wig, SIGNAL(valueChanged(QPair<int, QPointF>)),
            this, SLOT(recvSplineValue(QPair<int, QPointF>)));
			
	connect(wig, SIGNAL(leftControlChanged(QPair<int, QPointF>)),
            this, SLOT(recvSplineCv0(QPair<int, QPointF>)));
			
	connect(wig, SIGNAL(rightControlChanged(QPair<int, QPointF>)),
            this, SLOT(recvSplineCv1(QPair<int, QPointF>)));
			
	return wig;
}

QWidget* AttribWidget::shoEnumAttr(gar::Attrib* attr)
{
	gar::EnumAttrib* sattr = static_cast<gar::EnumAttrib*> (attr);
	
	QEnumCombo* wig = new QEnumCombo(tr(attr->attrNameStr().c_str() ) );
	wig->setNameId(attr->attrName() );
	
	const int& nf = sattr->numFields();
	for(int i=0;i<nf;++i) {
		const int& fi = sattr->getField(i);
		QString fnm(gar::Attrib::IntAsEnumFieldName(fi) );
		wig->addField(fnm, fi);
	}
	
	int val;
	sattr->getValue(val);
	wig->setValue(val);
	
	connect(wig, SIGNAL(valueChanged2(QPair<int, int>)),
            this, SLOT(recvEnumValue(QPair<int, int>)));
	
	return wig;
}

QWidget* AttribWidget::shoActionAttr(gar::Attrib* attr)
{
	gar::ActionAttrib* sattr = static_cast<gar::ActionAttrib*> (attr);
	
	QString lab = tr(attr->attrNameStr().c_str() );
	 	
	QIcon icn(tr(sattr->imageName().c_str()) );
	
	IconButtonGroup* wig = new IconButtonGroup(icn, lab);
	wig->setNameId(attr->attrName() );
	
	connect(wig, SIGNAL(buttonPressed2(QPair<int, int>)),
            this, SLOT(recvActionPressed(QPair<int, int>)));
	
	return wig;
}

void AttribWidget::recvVec2Value(QPair<int, QVector<double> > x)
{
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(x.first) );	
	if(!dst) {
		qDebug()<<" AttribWidget::recvDoubleValue cannot find float attr "
			<<x.first;
		return;
	}
	
	float vals[2];
	vals[0] = x.second[0];
	vals[1] = x.second[1];
	dst->setValue2(vals);
	updateSelectedGlyph();
}

void AttribWidget::recvDoubleValue(QPair<int, double> x)
{
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(x.first) );	
	if(!dst) {
		qDebug()<<" AttribWidget::recvDoubleValue cannot find float attr "
			<<x.first;
		return;
	}
	dst->setValue((float)x.second);
	updateSelectedGlyph();
}

void AttribWidget::recvIntValue(QPair<int, QVector<int> > x)
{
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(x.first) );	
	if(!dst) {
		qDebug()<<" AttribWidget::recvIntValue cannot find int attr "
			<<x.first;
		return;
	}
	dst->setValue(x.second[0]);
	updateSelectedGlyph();
}

void AttribWidget::recvInt2Value(QPair<int, QVector<int> > x)
{
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(x.first) );	
	if(!dst) {
		qDebug()<<" AttribWidget::recvInt2Value cannot find int2 attr "
			<<x.first;
		return;
	}
	int vals[2];
	vals[0] = x.second[0];
	vals[1] = x.second[1];
	dst->setValue2(vals);
	updateSelectedGlyph();
}

void AttribWidget::recvStringValue(QPair<int, QString> x)
{
	gar::StringAttrib* sattr = findStringAttr(x.first);
	if(!sattr) 
		return;
	sattr->setValue(x.second.toStdString() );
	updateSelectedGlyph();
}

gar::StringAttrib* AttribWidget::findStringAttr(int i)
{
	if(!m_selectedGlyph) 
		return NULL;
		
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(i) );	
	if(!dst) {
		qDebug()<<" AttribWidget cannot find string attr "
			<<i;
		return NULL;
	}
	
	return static_cast<gar::StringAttrib*> (dst);
}

gar::SplineAttrib* AttribWidget::findSplineAttr(int i)
{
	if(!m_selectedGlyph) 
		return NULL;
		
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(i) );	
	if(!dst) {
		qDebug()<<" AttribWidget cannot find spline attr "
			<<i;
		return NULL;
	}
	
	return static_cast<gar::SplineAttrib*> (dst);
}

gar::EnumAttrib* AttribWidget::findEnumAttr(int i)
{
	if(!m_selectedGlyph) 
		return NULL;
		
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(i) );	
	if(!dst) {
		qDebug()<<" AttribWidget cannot find enum attr "
			<<i;
		return NULL;
	}
	
	return static_cast<gar::EnumAttrib*> (dst);
}

gar::ActionAttrib* AttribWidget::findActionAttr(int i)
{
	if(!m_selectedGlyph) 
		return NULL;
		
	PieceAttrib* att = m_selectedGlyph->attrib();
	gar::Attrib* dst = att->findAttrib(gar::Attrib::IntAsAttribName(i) );	
	if(!dst) {
		qDebug()<<" AttribWidget cannot find action attr "
			<<i;
		return NULL;
	}
	
	return static_cast<gar::ActionAttrib*> (dst);
}

void AttribWidget::recvSplineValue(QPair<int, QPointF> x)
{
	gar::SplineAttrib* sattr = findSplineAttr(x.first);
	if(!sattr) 
		return;
	
	float tmp[2];
	QPointF& p = x.second;
	tmp[0] = p.x();
	tmp[1] = p.y();
	sattr->setSplineValue(tmp[0], tmp[1] );
	updateSelectedGlyph();
}
	
void AttribWidget::recvSplineCv0(QPair<int, QPointF> x)
{
	gar::SplineAttrib* sattr = findSplineAttr(x.first);
	if(!sattr) 
		return;
		
	float tmp[2];
	QPointF& p = x.second;
	tmp[0] = p.x();
	tmp[1] = p.y();
	sattr->setSplineCv0(tmp[0], tmp[1]);
	updateSelectedGlyph();
}
	
void AttribWidget::recvSplineCv1(QPair<int, QPointF> x)
{
	gar::SplineAttrib* sattr = findSplineAttr(x.first);
	if(!sattr) 
		return;
		
	float tmp[2];
	QPointF& p = x.second;
	tmp[0] = p.x();
	tmp[1] = p.y();
	sattr->setSplineCv1(tmp[0], tmp[1]);
	updateSelectedGlyph();
}

void AttribWidget::updateSelectedGlyph()
{
	if(!m_selectedGlyph) 
		return;
		
	PieceAttrib* att = m_selectedGlyph->attrib();
	if(att->update())
		emit sendAttribChanged();
}

void AttribWidget::recvEnumValue(QPair<int, int> x)
{
	gar::EnumAttrib* sattr = findEnumAttr(x.first);
	if(!sattr) 
		return;
	
	sattr->setValue(x.second);
	updateSelectedGlyph();
}

void AttribWidget::recvActionPressed(QPair<int, int> x)
{
	gar::ActionAttrib* sattr = findActionAttr(x.first);
	if(!sattr) 
		return;
	
	sattr->setValue(x.second);
	updateSelectedGlyph();
}

void AttribWidget::recvSelectGlyph(bool x)
{
	if(x) {
		GardenGlyph* g = m_scene->lastSelectedGlyph();
		if(g == m_selectedGlyph) {
			return;
		}
		m_selectedGlyph = g;
		clearAttribs();
		lsAttribs(g);
			
	} else {
		m_selectedGlyph = NULL;
		clearAttribs();
	}
}

QString AttribWidget::lastSelectedGlyphName() const
{
	GardenGlyph* g = m_scene->lastSelectedGlyph();
	if(!g)
		return tr("");
		
	return tr(g->glyphName().c_str() );
}

QString AttribWidget::lastSelectedGlyphTypeName() const
{
	QString stype(tr("unknown"));
	
	if(!m_selectedGlyph)
		return stype;
		
	const int& gt = m_selectedGlyph->glyphType();
	
	const int gg = gar::ToGroupType(gt );
	switch (gg) {
		case gar::ggGround :
			stype = gar::GroundTypeNames[gar::ToGroundType(gt)];
		break;
		case gar::ggGrass :
			stype = gar::GrassTypeNames[gar::ToGrassType(gt)];
		break;
		case gar::ggFile :
			stype = gar::FileTypeNames[gar::ToFileType(gt)];
		break;
		case gar::ggSprite :
			stype = gar::BillboardTypeNames[gar::ToBillboardType(gt)];
		break;
		case gar::ggVariant :
			stype = gar::VariationTypeNames[gar::ToVariationType(gt)];
		break;
		case gar::ggStem :
			stype = gar::StemTypeNames[gar::ToStemType(gt)];
		break;
		case gar::ggTwig :
			stype = gar::TwigTypeNames[gar::ToTwigType(gt)];
		break;
		case gar::ggBranch :
			stype = gar::BranchTypeNames[gar::ToBranchType(gt)];
		break;
		default:
		;
	}
	return stype;
}
