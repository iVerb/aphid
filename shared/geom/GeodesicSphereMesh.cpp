/*
 *  CubeMesh.cpp
 *  brdf
 *
 *  Created by jian zhang on 9/30/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>
#include "GeodesicSphereMesh.h"
#include <math/ATypes.h>
#include <math/miscfuncs.h>

namespace aphid {
    
TriangleGeodesicSphere::TriangleGeodesicSphere(int level)
{
    unsigned np = (level + 1) * (level + 1) * 4;
    unsigned nt = level * level * 2 * 4;
    create(np, nt);
    
    Vector3F * p = points();
	unsigned * idx = indices();
	
	unsigned currentidx = 0;
	unsigned currentver = 0;
	
	const Vector3F a(0.f, 1.f, 0.f);
	const Vector3F b(-1.f, 0.f, 0.f);
	const Vector3F c(0.f, 0.f, 1.f);
	const Vector3F d(1.f, 0.f, 0.f);
	const Vector3F e(0.f, 0.f, -1.f);
	const Vector3F f(0.f, -1.f, 0.f);
	
	subdivide(level, currentver, currentidx, p, idx, a, b, c, d);
	subdivide(level, currentver, currentidx, p, idx, a, d, e, b);
	subdivide(level, currentver, currentidx, p, idx, f, d, c, b);
	subdivide(level, currentver, currentidx, p, idx, f, b, e, d);

	for(unsigned i=0;i<np;++i) {
	     p[i].normalize();
	     vertexNormals()[i] = p[i];
	}
}

TriangleGeodesicSphere::~TriangleGeodesicSphere()
{}

void TriangleGeodesicSphere::subdivide(int level, unsigned & currentVertex, unsigned & currentIndex, 
        Vector3F * p, unsigned * idx, 
        const Vector3F & a, const Vector3F & b, const Vector3F & c, const Vector3F & d)
{
    unsigned offset = currentVertex;
    Vector3F delta_ab = (b - a) / (float)level;
    Vector3F delta_bc = (c - b) / (float)level;
    Vector3F delta_ad = (d - a) / (float)level;
    for(int j = 0; j <= level; j++)
    {
        Vector3F row = a + delta_ab * (float)j;
        p[currentVertex] = row;           
        currentVertex++; 
        for(int i = 1; i <= level; i++)
        {
            if(i <= j) row += delta_bc;
            else row += delta_ad;
            
            p[currentVertex] = row;           
            currentVertex++; 
        }
    }
    
    for(int j = 0; j < level; j++)
    {
        for(int i = 0; i < level; i++)
        {
            idx[currentIndex] = j * (level + 1) + i + offset;           
            currentIndex++;
            
            idx[currentIndex] = (j + 1) * (level + 1) + i + offset;           
            currentIndex++;
			
			idx[currentIndex] = (j + 1) * (level + 1) + (i + 1) + offset;           
            currentIndex++;
            
            idx[currentIndex] = j * (level + 1) + i + offset;           
            currentIndex++;
            
            idx[currentIndex] = (j + 1) * (level + 1) + (i + 1) + offset;           
            currentIndex++;
			
			idx[currentIndex] = j * (level + 1) + i + 1 + offset;           
            currentIndex++;
        }
    }
}

void TriangleGeodesicSphere::computeSphericalCoord(Float2* dst) const
{

	const Vector3F * src = vertexNormals();
	
	const int n = numPoints();
	for(int i=0;i<n;++i) {
		const Vector3F & pcat = src[i];
		Float2 & ppl = dst[i];
/// phi
		if(Absolute<float>(pcat.y) < .01f) {
			if(pcat.x >= 0.f)
				ppl.x = 0.f;
			else
				ppl.x = PIF;
				
		} else {
			ppl.x = acos(pcat.x / sqrt(pcat.x * pcat.x + pcat.y * pcat.y ) );
			if(pcat.y < 0.f)
				ppl.x = -ppl.x;
		}
/// theta
		ppl.y = asin(pcat.z);
		
	}
}
    

GeodesicSphereMesh::GeodesicSphereMesh(unsigned level)
{
    unsigned nv = (level + 1) * (level + 1) * 4;
    
    unsigned nf = level * level * 2 * 4;
    
    createVertices(nv);
	createIndices(nf * 3);

	Vector3F * p = vertices();
	
	unsigned * idx = indices();
	
	unsigned currentidx = 0;
	unsigned currentver = 0;
	
	Vector3F a(0.f, 1.f, 0.f);
	Vector3F b(-1.f, 0.f, 0.f);
	Vector3F c(0.f, 0.f, 1.f);
	Vector3F d(1.f, 0.f, 0.f);
	Vector3F e(0.f, 0.f, -1.f);
	Vector3F f(0.f, -1.f, 0.f);
	
	subdivide(level, currentver, currentidx, p, idx, a, b, c, d);
	subdivide(level, currentver, currentidx, p, idx, a, d, e, b);
	subdivide(level, currentver, currentidx, p, idx, f, d, c, b);
	subdivide(level, currentver, currentidx, p, idx, f, b, e, d);

    setRadius(1.f);
}

GeodesicSphereMesh::~GeodesicSphereMesh() {}

void GeodesicSphereMesh::setRadius(float r)
{
	const unsigned nv = getNumVertices();
	Vector3F * p = vertices();
	for(unsigned i = 0; i < nv; i++) {
		p[i].normalize();
		p[i] *= r;
	}
}

void GeodesicSphereMesh::subdivide(unsigned level, unsigned & currentVertex, unsigned & currentIndex, Vector3F * p, unsigned * idx, Vector3F a, Vector3F b, Vector3F c, Vector3F d)
{
    unsigned offset = currentVertex;
    Vector3F delta_ab = (b - a) / (float)level;
    Vector3F delta_bc = (c - b) / (float)level;
    Vector3F delta_ad = (d - a) / (float)level;
    for(unsigned j = 0; j <= level; j++)
    {
        Vector3F row = a + delta_ab * (float)j;
        p[currentVertex] = row;           
        currentVertex++; 
        for(unsigned i = 1; i <= level; i++)
        {
            if(i <= j) row += delta_bc;
            else row += delta_ad;
            
            p[currentVertex] = row;           
            currentVertex++; 
        }
    }
    
    for(unsigned j = 0; j < level; j++)
    {
        for(unsigned i = 0; i < level; i++)
        {
            idx[currentIndex] = j * (level + 1) + i + offset;           
            currentIndex++;
            
            idx[currentIndex] = (j + 1) * (level + 1) + i + offset;           
            currentIndex++;
			
			idx[currentIndex] = (j + 1) * (level + 1) + (i + 1) + offset;           
            currentIndex++;
            
            idx[currentIndex] = j * (level + 1) + i + offset;           
            currentIndex++;
            
            idx[currentIndex] = (j + 1) * (level + 1) + (i + 1) + offset;           
            currentIndex++;
			
			idx[currentIndex] = j * (level + 1) + i + 1 + offset;           
            currentIndex++;
        }
    }
}

}