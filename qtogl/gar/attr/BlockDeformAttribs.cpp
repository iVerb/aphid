/*
 *  BlockDeformAttribs.cpp
 *  
 *
 *  Created by jian zhang on 8/6/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "BlockDeformAttribs.h"
#include <geom/ATriangleMesh.h>
#include <geom/BlockDeformer.h>
#include <math/miscfuncs.h>
#include <gar_common.h>

using namespace aphid;

int BlockDeformAttribs::sNumInstances = 0;

BlockDeformAttribs::BlockDeformAttribs() : PieceAttrib(gar::gtBlockDeformVariant),
m_inAttr(NULL),
m_inGeom(NULL),
m_exclR(1.f)
{
    m_instId = sNumInstances;
	sNumInstances++;
	
	addVector2Attrib(gar::nBend, 0.2f, -.2f);
	addFloatAttrib(gar::nTwist, 0.1f, 0.f, 1.f);
	addFloatAttrib(gar::nRoll, 0.1f, 0.f, 1.f);
	addFloatAttrib(gar::nLengthScale, 1.f, 0.1f, 100.f);
	addFloatAttrib(gar::nRadiusScale, 1.f, 0.1f, 100.f);

	m_dfm = new BlockDeformer;	
	for(int i=0;i<48;++i) 
	    m_outGeom[i] = new ATriangleMesh;
}

bool BlockDeformAttribs::hasGeom() const
{ return m_inGeom != NULL; }

int BlockDeformAttribs::numGeomVariations() const
{ return 48; }

ATriangleMesh* BlockDeformAttribs::selectGeom(gar::SelectProfile* prof) const
{
	if(prof->_condition == gar::slRandom)
		prof->_index = rand() % numGeomVariations();
	else if(prof->_condition == gar::slCloseToUp)
		prof->_index = variationCloseToUp(prof);
		
    prof->_exclR = m_exclR;
	prof->_height = m_geomHeight;
	return m_outGeom[prof->_index]; 
}

bool BlockDeformAttribs::update()
{    
    if(!m_inAttr)
        return false;
    
	gar::SelectProfile selprof;
    m_inGeom = m_inAttr->selectGeom(&selprof);
    
    if(!m_inGeom)
        return false;
		
	m_exclR = selprof._exclR;
	m_geomHeight = selprof._height;
		
	BlockDeformerBuilder builder;
/// a chain of 8 blocks
	const float deltaHeight = m_geomHeight * .125f;
	builder.setYSeg(deltaHeight);
	
	float scaling2[2];
	findAttrib(gar::nLengthScale)->getValue(scaling2[0]);
	findAttrib(gar::nRadiusScale)->getValue(scaling2[1]);
	m_dfm->setScaling(scaling2);
	
	m_exclR *= scaling2[1];
	m_geomHeight *= scaling2[0];
	
/// root at origin
	deform::Block* curBlock = new deform::Block;
	Matrix44F* ltm = curBlock->tmR();
	ltm->setIdentity();
	builder.addBlock(curBlock);
	
	deform::Block* lastBlock = curBlock;
	for(int i=1;i<8;++i) {
		curBlock = new deform::Block(lastBlock);
/// local translation of each block
		ltm = curBlock->tmR();
		ltm->setTranslation(0.f, deltaHeight * scaling2[0], 0.f);
		
		builder.addBlock(curBlock);
		lastBlock = curBlock;
	}
	m_dfm->createBlockDeformer(m_inGeom, builder);
	
    float bendRange[2];
	findAttrib(gar::nBend)->getValue2(bendRange);
	
	float twistRoll[2];
	findAttrib(gar::nTwist)->getValue(twistRoll[0]);
	findAttrib(gar::nRoll)->getValue(twistRoll[1]);
	
/// 6 bend groups
	const float deltaBend = (bendRange[1] - bendRange[0]) / 5.f;
/// 8 roll per group
	const float deltaRoll = twistRoll[1] / 3.5f;
		
	float angles[3];
    for(int i=0;i<6;++i) {
        for(int j=0;j<8;++j) {
            angles[1] = twistRoll[0] * (.5f + .5f* RandomF01() );
            
            if(j & 1) {
                angles[1] = -angles[1];
            }
/// roll distribution
            angles[2] = deltaRoll * (RandomFn11() + j) - twistRoll[1];

			angles[0] = bendRange[0] + deltaBend * (RandomFn11() + i);
			m_dfm->setBend(angles[0]);
            m_dfm->setTwist(angles[1]);
            m_dfm->setRoll(angles[2]);
            m_dfm->deform(m_inGeom);
            m_dfm->updateGeom(m_outGeom[i * 8  + j], m_inGeom);
			m_dfm->getBlockTms(m_blkMat[i * 8  + j]);
		}
	}
	
	computeTexcoord(m_outGeom, 48, m_inAttr->texcoordBlockAspectRatio() );
	
	return true;
}

int BlockDeformAttribs::attribInstanceId() const
{ return m_instId; }

void BlockDeformAttribs::connectTo(PieceAttrib* another, const std::string& portName)
{
    if(!another->hasGeom()) {
        std::cout<<"\n ERROR BlockDeformAttribs cannot connect input geom ";
        m_inGeom = NULL;  
        return;
    }
    
    m_inAttr = another;
    update();
}

void BlockDeformAttribs::disconnectFrom(PieceAttrib* another, const std::string& portName)
{
	m_inAttr = NULL;
	m_inGeom = NULL;
}

bool BlockDeformAttribs::isGeomStem() const
{
	if(!m_inAttr)
		return false;
	return m_inAttr->isGeomStem();
}

bool BlockDeformAttribs::isGeomLeaf() const
{
	if(!m_inAttr)
		return false;
	return m_inAttr->isGeomLeaf();
}

bool BlockDeformAttribs::isGeomBranchingUnit() const
{
	if(!m_inAttr)
		return false;
	return m_inAttr->isGeomStem();
}

gar::BranchingUnitType BlockDeformAttribs::getBranchingUnitType() const
{
	if(!m_inAttr)
		return gar::buUnknown;
	return m_inAttr->getBranchingUnitType();
}

bool BlockDeformAttribs::canConnectToViaPort(const PieceAttrib* another, const std::string& portName) const
{
	return (another->isGeomStem() || another->isGeomLeaf() );
}

bool BlockDeformAttribs::selectBud(gar::SelectBudContext* ctx) const
{
	if(!m_inAttr)
		return false;
				
	if(!m_inAttr->selectBud(ctx) )
		return false;
		
	if(ctx->_budType == gar::bdTerminal
		|| ctx->_budType == gar::bdTerminalFoliage )
		return selectTerminalBud(ctx);
	
	return selectLateralBud(ctx);
}

bool BlockDeformAttribs::selectTerminalBud(gar::SelectBudContext* ctx) const
{
	const Vector3F relup = getLocalUpRef(ctx);
	
	const float* vartm = m_blkMat[ctx->_variationIndex];
	const int& bindi = ctx->_budBind[0];
	Matrix44F tm(&vartm[bindi<<4]);
	Matrix44F loctm(ctx->_budTm[0]);
	loctm *= tm;
	if(ctx->_budType == gar::bdTerminalFoliage) {
		rotateToUp(loctm, relup);
	}
	loctm.glMatrix(ctx->_budTm[0]);
	return true;
}

bool BlockDeformAttribs::selectLateralBud(gar::SelectBudContext* ctx) const
{
	if(ctx->_condition == gar::slAll)
		return selectAllLateralBud(ctx);
		
	return selectCloseToUpLateralBud(ctx);
}

bool BlockDeformAttribs::selectAllLateralBud(gar::SelectBudContext* ctx) const
{
	const Vector3F relup = getLocalUpRef(ctx);

	const float* vartm = m_blkMat[ctx->_variationIndex];
	for(int i=0;i<ctx->_numSelect;++i) {
		const int& bindi = ctx->_budBind[i];
		Matrix44F tm(&vartm[bindi<<4]);
		Matrix44F loctm(ctx->_budTm[i]);
		loctm *= tm;
		if(ctx->_budType == gar::bdLateralFoliage) {
		
			Vector3F wup = tm.getUp();
			wup.normalize();
			rotateToUp2(loctm, relup, wup);
		}
		loctm.glMatrix(ctx->_budTm[i]);
	}
	
	return true;
}
	
bool BlockDeformAttribs::selectCloseToUpLateralBud(gar::SelectBudContext* ctx) const
{		
	const Matrix44F wtm(ctx->_relMat);
	const Vector3F upref(ctx->_upVec);
		
	const float* vartm = m_blkMat[ctx->_variationIndex];
	
	int nup = 0;
	for(int i=0;i<ctx->_numSelect;++i) {
		const int& bindi = ctx->_budBind[i];
		Matrix44F tm(&vartm[bindi<<4]);
		Matrix44F loctm(ctx->_budTm[i]);
		loctm *= tm;
		const Vector3F locup = wtm.transformAsNormal(loctm.getUp() );
		if(locup.dot(upref) > ctx->_upLimit) {
/// facing up
			loctm.glMatrix(ctx->_budTm[nup]);
			nup++;
		}
	}
	ctx->_numSelect = nup;
	
	return true;
}

Vector3F BlockDeformAttribs::variationDirection(int i) const
{ 
	const float* mati = &m_blkMat[i][112];
	return Vector3F(mati[4], mati[5], mati[6]); 
}

int BlockDeformAttribs::variationCloseToUp(gar::SelectProfile* prof) const
{
	const Vector3F vref(prof->_upVec);
	const Matrix44F tm(prof->_relMat);
	int ind = 0;
	float maxD = -1e8f;
	for(int i=0;i<numGeomVariations();++i) {
		const Vector3F vi = tm.transformAsNormal(variationDirection(i) );
		float d = vi.dot(vref);
		if(maxD < d) {
			maxD = d;
			ind = i;
		}
	}
	return ind;
}

void BlockDeformAttribs::rotateToUp(aphid::Matrix44F& tm, const Vector3F& relup) const
{
	Vector3F locf = tm.getFront();
	locf.normalize();

/// front is close to up
	if(locf.dot(relup) > .9f)
		return;
		
	Vector3F locup = tm.getUp();
	locup.normalize();
	
	const float udu = locup.dot(relup);
	if(udu > .9f || udu < -.9f)
/// undefined angle
		return;
		
	Vector3F ucu = locup.cross(relup);
	ucu.normalize();
	
	Vector3F tfront = ucu.cross(locup);
	tfront.normalize();
	
	float fdf = locf.dot(tfront);
	if(fdf > .9f)
		return;
		
	if(fdf < -.9f) {
/// back facing to up
		Quaternion qy(3.14f, locup);
		Matrix33F roty(qy);
		tm *= roty;
		return;
	}
	
	float ang = acos(fdf);
	Vector3F rotax = locf.cross(tfront);
	rotax.normalize();
	
	Quaternion qya(ang, rotax);
	Matrix33F rotya(qya);
	tm *= rotya;
}

void BlockDeformAttribs::rotateToUp2(aphid::Matrix44F& tm, const aphid::Vector3F& relup,
							const aphid::Vector3F& yaxis) const
{
	Vector3F locf = tm.getFront();
	locf.normalize();

/// front is close to up
	if(locf.dot(relup) > .9f)
		return;
		
	Vector3F locup = tm.getUp();
	
/// project to x-z
	Vector3F locupxz(locup.x, 0.f, locup.z);
	locupxz.normalize();
	
	Vector3F refupxz(relup.x, 0.f, relup.z);
	refupxz.normalize();
	
	const float uduxz = locupxz.dot(refupxz);
	
	if(uduxz > .9f || uduxz < -.9f) {
		Quaternion qy(.789f, yaxis );
		Matrix33F rot(qy);
		tm *= rot;
				
	} else if(uduxz > .707f) {
	
		float ang = 1.57f - acos(uduxz);
		if(ang > .789f)
			ang = .789f;
			
		Quaternion qy(ang, yaxis );
		Matrix33F rot(qy);
		tm *= rot;
		
	} else if(uduxz < -.707f) {
	
		float ang = acos(uduxz) - 1.57f;
		if(ang > .789f)
			ang = .789f;
		
		Quaternion qy(ang, yaxis );
		Matrix33F rot(qy);
		tm *= rot;
		
	}

/// update y-axis	
	locup = tm.getUp();
	locup.normalize();
	
	Vector3F locfront = tm.getFront();
	locfront.normalize();
	
	float fdu = locfront.dot(relup);
/// close to up
	if(fdu > .9f)
		return;
		
	if(fdu < -.9f) {
/// back side facing up, flip
		Quaternion qy(3.14f, locup );
		Matrix33F rot(qy);
		tm *= rot;
		return;
	}
	
/// rotate around y-axis half way to up
	float hang = .53f * acos(fdu);
	if(hang > .789f)
		hang = .789f;
	
	Vector3F ucu = locup.cross(relup);
	if(ucu.dot(locfront) > 0.f ) {
		hang = -hang;
	}
	
	Quaternion qu(hang, locup );
	Matrix33F rot(qu);
	tm *= rot;
	
}

void BlockDeformAttribs::estimateExclusionRadius(float& minRadius)
{
	if(m_inAttr)
		m_inAttr->estimateExclusionRadius(minRadius);
}
