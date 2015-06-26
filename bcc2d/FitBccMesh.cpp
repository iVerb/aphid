/*
 *  FitBccMesh.cpp
 *  testbcc
 *
 *  Created by jian zhang on 4/27/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include "FitBccMesh.h"
#include <BccGlobal.h>
#include "FitBccMeshBuilder.h"
FitBccMesh::FitBccMesh() 
{
}

FitBccMesh::~FitBccMesh() 
{
}

void FitBccMesh::create(GeometryArray * geoa, KdIntersection * anchorIntersect)
{
	std::vector<Vector3F > tetrahedronP;
	std::vector<unsigned > tetrahedronInd;
	
	FitBccMeshBuilder builder;
	builder.build(geoa, tetrahedronP, tetrahedronInd);
	
	unsigned nt = tetrahedronInd.size()/4;
	unsigned np = tetrahedronP.size();
	
	// nt = 16;
	// np = 18;
	
	//std::cout<<" n tetrahedrons "<<nt<<"\n";
	//std::cout<<" n vertices "<<np<<"\n";
	
	setNumPoints(np);
	setNumIndices(nt * 4);
	createBuffer(np, nt * 4);
	
	unsigned i;
	for(i=0;i<np;i++) points()[i] = tetrahedronP[i];
	
	for(i=0;i<nt*4;i++) indices()[i] = tetrahedronInd[i];
	
	resetAnchors(np);
	
	addAnchors(builder.startPoints(), geoa->numGeometries());
}

void FitBccMesh::addAnchors(KdIntersection * anchorIntersect)
{
	BoundingBox box;
	unsigned j, i=0;
	for(; i< numTetrahedrons(); i++) {
        unsigned * tet = tetrahedronIndices(i);
		
		box.reset();
        for(j=0; j< 4; j++)
            box.expandBy(points()[tet[j]], 0.001f); 
        
		if(!anchorIntersect->intersectBox(box)) continue;
		
		for(j=0; j< 4; j++) {
			anchors()[tet[j]] = 1;
		}
    }
}

void FitBccMesh::addAnchors(Vector3F * anchorPoints, unsigned n)
{
	unsigned i, j, k;
	unsigned * anchorTri = new unsigned[n];
	for(i=0; i< n; i++) {
// todo: find closest tri id for each start point
	}
	
	BoundingBox box;
	int anchored;
	for(i=0; i< numTetrahedrons(); i++) {
        unsigned * tet = tetrahedronIndices(i);
		
		box.reset();
        for(j=0; j< 4; j++)
            box.expandBy(points()[tet[j]], 0.001f); 
        
		// if(!anchorIntersect->intersectBox(box)) continue;
		anchored = 0;
		for(k=0; k<n; k++) {
			if(box.isPointInside(anchorPoints[k])) {
				anchored = 1;
				break;
			}
		}
		
		if(!anchored) continue;
		
		for(j=0; j< 4; j++) {
			anchors()[tet[j]] = 1;
		}
    }
	delete[] anchorTri;
}
//:~