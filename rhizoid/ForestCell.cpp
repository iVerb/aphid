/*
 *  ForestCell.cpp
 *  proxyPaint
 *
 *  Created by jian zhang on 1/19/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */

#include "ForestCell.h"
#include <PlantCommon.h>

namespace aphid {

ForestCell::ForestCell(Entity * parent) : sdb::Array<sdb::Coord2, Plant>(parent)
{
	m_lodsamp = new sdb::LodSampleCache;
	m_numActiveSamples = 0;
	m_numVisibleSamples = 0;
}

ForestCell::~ForestCell()
{
	m_activeInd.clear();
	delete m_lodsamp;
}

const sdb::SampleCache * ForestCell::sampleAtLevel(int level) const
{ return m_lodsamp->samplesAtLevel(level); }

const float * ForestCell::samplePoints(int level) const
{ return sampleAtLevel(level)->points(); }

const float * ForestCell::sampleNormals(int level) const
{ return sampleAtLevel(level)->normals(); }

const float * ForestCell::sampleColors(int level) const
{ return sampleAtLevel(level)->colors(); }

const int & ForestCell::numSamples(int level) const
{ return m_lodsamp->numSamplesAtLevel(level); }

bool ForestCell::hasSamples(int level) const
{ return m_lodsamp->hasLevel(level); }

void ForestCell::deselectSamples()
{
	m_activeInd.clear();
	m_numActiveSamples = 0;
	m_numVisibleSamples = 0;
}

void ForestCell::updateIndices(int & count, int * indices,
			sdb::Sequence<int> & srcInd)
{
	count = srcInd.size();
	if(count < 1) {
		return;
	}
	
	int c=0;
	srcInd.begin();
	while(!srcInd.end() ) {
	
		indices[c] = srcInd.key();
		c++;
		
		srcInd.next();
	}
}

void ForestCell::clearSamples()
{
	m_lodsamp->clear();
}

const int & ForestCell::numActiveSamples() const
{ return m_numActiveSamples; }

const int * ForestCell::activeSampleIndices() const
{ return m_activeSampleIndices.get(); }

const int & ForestCell::numVisibleSamples() const
{ return m_numVisibleSamples; }

const int * ForestCell::visibleSampleIndices() const
{ return m_visibleSampleIndices.get(); }

void ForestCell::reshuffleSamples(const int & level)
{ m_lodsamp->reshuffleAtLevel(level); }

}
