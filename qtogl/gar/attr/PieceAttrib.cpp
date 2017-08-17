/*
 *  PieceAttrib.cpp
 *  
 *
 *  Created by jian zhang on 8/5/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "PieceAttrib.h"
#include <gar_common.h>
#include <math/SplineMap1D.h>

using namespace aphid;

PieceAttrib::PieceAttrib(int glyphType) :
m_glyphType(glyphType)
{
	char b[17];
    gar::GenGlyphName(b);
	m_glyphName = std::string(b);
	addStringAttrib(gar::nmNodeName, m_glyphName);
}

PieceAttrib::~PieceAttrib()
{
	AttribArrayTyp::iterator it = m_collAttrs.begin();
	for(;it!=m_collAttrs.end();++it) {
		delete *it;
	}
	m_collAttrs.clear();
}

const std::string& PieceAttrib::glyphName() const
{ return m_glyphName; }

const int& PieceAttrib::glyphType() const
{ return m_glyphType; }

void PieceAttrib::addIntAttrib(gar::AttribName anm,
		const int& val, 
		const int& minVal,
		const int& maxVal)
{
	gar::Attrib* aat = new gar::Attrib(anm, gar::tInt);
	aat->setValue(val);
	aat->setMin(minVal);
	aat->setMax(maxVal);
	m_collAttrs.push_back(aat);
}	

void PieceAttrib::addFloatAttrib(gar::AttribName anm,
		const float& val, 
		const float& minVal,
		const float& maxVal)
{
	gar::Attrib* aat = new gar::Attrib(anm, gar::tFloat);
	aat->setValue(val);
	aat->setMin(minVal);
	aat->setMax(maxVal);
	m_collAttrs.push_back(aat);	
}

void PieceAttrib::addVector2Attrib(gar::AttribName anm,
		const float& val0, 
		const float& val1)
{
	gar::Attrib* aat = new gar::Attrib(anm, gar::tVec2);
	float tmp[2]; 
	tmp[0] = val0;
	tmp[1] = val1;
		
	aat->setValue2(tmp);
	m_collAttrs.push_back(aat);
}

void PieceAttrib::addSplineAttrib(gar::AttribName anm)
{
	gar::SplineAttrib* aat = new gar::SplineAttrib(anm);
	m_collAttrs.push_back(aat);
}

void PieceAttrib::addStringAttrib(gar::AttribName anm,
		const std::string& val,
		const bool& asFileName)
{
	gar::StringAttrib* aat = new gar::StringAttrib(anm, asFileName);
	aat->setValue(val);
	m_collAttrs.push_back(aat);
}

void PieceAttrib::addEnumAttrib(gar::AttribName anm,
			const std::vector<int>& fields)
{
	gar::EnumAttrib* aat = new gar::EnumAttrib(anm);
	
	int nf = fields.size();
	aat->createFields(nf);
	for(int i=0;i<nf;++i) {
		aat->setField(i, fields[i]);
	}
/// first is default
	aat->setValue(fields[0]);
	m_collAttrs.push_back(aat);
}

int PieceAttrib::numAttribs() const
{ return m_collAttrs.size(); }

gar::Attrib* PieceAttrib::getAttrib(const int& i)
{ return m_collAttrs[i]; }

const gar::Attrib* PieceAttrib::getAttrib(const int& i) const
{ return m_collAttrs[i]; }

gar::Attrib* PieceAttrib::findAttrib(gar::AttribName anm)
{
	AttribArrayTyp::iterator it = m_collAttrs.begin();
	for(;it!=m_collAttrs.end();++it) {
		if( (*it)->attrName() == anm)
			return *it;
	}
	std::cout<<"\n ERROR cannot find attrib "<<anm;
	std::cout.flush();
	return NULL;
}

const gar::Attrib* PieceAttrib::findAttrib(gar::AttribName anm) const
{
	AttribArrayTyp::const_iterator it = m_collAttrs.begin();
	for(;it!=m_collAttrs.end();++it) {
		if( (*it)->attrName() == anm)
			return *it;
	}
	std::cout<<"\n ERROR cannot find attrib "<<anm;
	std::cout.flush();
	return NULL;
}

bool PieceAttrib::hasGeom() const
{ return false; }
	
int PieceAttrib::numGeomVariations() const
{ return 0; }

aphid::ATriangleMesh* PieceAttrib::selectGeom(gar::SelectProfile* prof) const
{ return NULL; }

bool PieceAttrib::update()
{ return false; }

int PieceAttrib::attribInstanceId() const
{ return 0; }

bool PieceAttrib::canConnectToViaPort(const PieceAttrib* another, const std::string& portName) const
{ return true; }

void PieceAttrib::connectTo(PieceAttrib* another, const std::string& portName)
{}

float PieceAttrib::texcoordBlockAspectRatio() const
{ return 1.f; }

void PieceAttrib::updateSplineValues(SplineMap1D* ls, gar::SplineAttrib* als)
{
	float tmp[2];
	als->getSplineValue(tmp);
	ls->setStart(tmp[0]);
	ls->setEnd(tmp[1]);
	
	als->getSplineCv0(tmp);
	ls->setLeftControl(tmp[0], tmp[1]);
	
	als->getSplineCv1(tmp);
	ls->setRightControl(tmp[0], tmp[1]);
}

bool PieceAttrib::isSynthesized() const
{ return false; }

int PieceAttrib::numSynthesizedGroups() const
{ return 0; }

gar::SynthesisGroup* PieceAttrib::synthesisGroup(int i) const
{ return NULL; }

bool PieceAttrib::isGeomStem() const
{ return false; }
bool PieceAttrib::isGeomLeaf() const
{ return false; }
