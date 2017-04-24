/*
 *  PlantPiece.cpp
 *  garden
 *
 *  Created by jian zhang on 4/15/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "PlantPiece.h"
#include <geom/ATriangleMesh.h>

using namespace aphid;

PlantPiece::PlantPiece(PlantPiece * parent) :
m_parentPiece(parent),
m_geom(NULL)
{
	if(parent) {
		parent->addBranch(this);
	}
}

PlantPiece::~PlantPiece()
{
	ChildListTyp::iterator it = m_childPieces.begin();
	for(;it!=m_childPieces.end();++it) {
		delete *it;
	}
	m_childPieces.clear();
}

void PlantPiece::addBranch(PlantPiece * c)
{ m_childPieces.push_back(c); }

void PlantPiece::setTransformMatrix(const Matrix44F &tm)
{ m_tm = tm; }

const Matrix44F & PlantPiece::transformMatrix() const
{ return m_tm; }

int PlantPiece::numBranches() const
{ return m_childPieces.size(); }

const PlantPiece * PlantPiece::branch(const int & i) const
{ return m_childPieces[i]; }

void PlantPiece::setGeometry(aphid::ATriangleMesh * geom)
{ m_geom = geom; }

const ATriangleMesh * PlantPiece::geometry() const
{ return m_geom; }
