/*
 *  ATriangleMesh.cpp
 *  aphid
 *
 *  Created by jian zhang on 4/25/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include <geom/ATriangleMesh.h>
#include <math/BaseBuffer.h>
#include <math/BarycentricCoordinate.h>
#include <geom/ClosestToPointTest.h>

namespace aphid {

ATriangleMesh::ATriangleMesh() 
{}

ATriangleMesh::~ATriangleMesh() 
{}

const TypedEntity::Type ATriangleMesh::type() const
{ return TTriangleMesh; }

const unsigned ATriangleMesh::numComponents() const
{ return numTriangles(); }

const unsigned ATriangleMesh::numTriangles() const
{ return numIndices() / 3; }

const BoundingBox ATriangleMesh::calculateBBox(unsigned icomponent) const
{
	Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	BoundingBox box;
	box.updateMin(p[v[0]]);
	box.updateMax(p[v[0]]);
	box.updateMin(p[v[1]]);
	box.updateMax(p[v[1]]);
	box.updateMin(p[v[2]]);
	box.updateMax(p[v[2]]);
	return box;
}

void ATriangleMesh::create(unsigned np, unsigned nt)
{
	createBuffer(np, nt * 3);
	setNumPoints(np);
	setNumIndices(nt * 3);
	m_texcoord.reset(new Float2[nt * 3]);
}

unsigned * ATriangleMesh::triangleIndices(unsigned idx) const
{ return &indices()[idx*3]; }

void ATriangleMesh::closestToPoint(unsigned icomponent, ClosestToPointTestResult * result)
{
	Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	
	BarycentricCoordinate & bar = result->_bar;
	bar.create(p[v[0]], p[v[1]], p[v[2]]);
	float d = bar.project(result->_toPoint);
	if(d>=result->_distance) return;
	bar.compute();
	if(!bar.insideTriangle()) bar.computeClosest();
	
	Vector3F clampledP = bar.getClosest();
	d = (clampledP - result->_toPoint).length();
	if(d>=result->_distance) return;
	
	result->_distance = d;
	result->_hasResult = true;
	result->_hitPoint = clampledP;
	result->_contributes[0] = bar.getV(0);
	result->_contributes[1] = bar.getV(1);
	result->_contributes[2] = bar.getV(2);
	result->_hitNormal = bar.getNormal();
	result->_icomponent = icomponent;
}

const Vector3F ATriangleMesh::triangleCenter(unsigned idx) const
{
	unsigned * v = triangleIndices(idx);
	Vector3F * p = points();
	return (p[v[0]] + p[v[1]] + p[v[2]]) / 3.f;
}

const Vector3F ATriangleMesh::triangleNormal(unsigned idx) const
{
	unsigned * v = triangleIndices(idx);
	Vector3F * p = points();
	Vector3F a = p[v[0]];
	Vector3F b = p[v[1]];
	Vector3F c = p[v[2]];
	Vector3F ab = b - a;
	Vector3F ac = c - a;
	Vector3F nor = ab.cross(ac);
	nor.normalize();
	return nor;
}

std::string ATriangleMesh::verbosestr() const
{
	std::stringstream sst;
	sst<<" triangle mesh nv "<<numPoints()
		<<"\n ntri "<<numTriangles()
		<<"\n";
	return sst.str();
}

bool ATriangleMesh::intersectTetrahedron(unsigned icomponent, const Vector3F * tet)
{
    Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	return gjk::IntersectTest::evaluateTriangle(p, v);
}

bool ATriangleMesh::intersectRay(unsigned icomponent, const Ray * r,
					Vector3F & hitP, Vector3F & hitN, float & hitDistance )
{
	Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	Vector3F a = p[v[0]];
	Vector3F b = p[v[1]];
	Vector3F c = p[v[2]];
	Vector3F ab = b - a;
	Vector3F ac = c - a;
	Vector3F nor = ab.cross(ac);
	nor.normalize();
	
	float ddotn = r->m_dir.dot(nor);
	
	//if(!ctx->twoSided && ddotn > 0.f) return 0;
	
	float t = (a.dot(nor) - r->m_origin.dot(nor)) / ddotn;
	
	if(t < 0.f || t > hitDistance) return 0;
	
	Vector3F onplane = r->m_origin + r->m_dir * t;
	Vector3F e01 = b - a;
	Vector3F x0 = onplane - a;
	if(e01.cross(x0).dot(nor) < 0.f) return 0;
	
	//printf("pass a\n");

	Vector3F e12 = c - b;
	Vector3F x1 = onplane - b;
	if(e12.cross(x1).dot(nor) < 0.f) return 0;
	
	//printf("pass b\n");
	
	Vector3F e20 = a - c;
	Vector3F x2 = onplane - c;
	if(e20.cross(x2).dot(nor) < 0.f) return 0;
	
	//printf("pass c\n");
	hitP = onplane;
	hitN = nor;
	hitDistance = t;
	
	return 1;
}

bool ATriangleMesh::intersectSphere(unsigned icomponent, const gjk::Sphere & B)
{ 
	Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	m_componentTriangle.x()[0] = p[v[0]];
	m_componentTriangle.x()[1] = p[v[1]];
	m_componentTriangle.x()[2] = p[v[2]];
	
	return gjk::Intersect1<gjk::TriangleSet, gjk::Sphere>::Evaluate(m_componentTriangle, B);
}

bool ATriangleMesh::intersectBox(unsigned icomponent, const BoundingBox & box)
{
	Vector3F * p = points();
	unsigned * v = triangleIndices(icomponent);
	m_componentTriangle.x()[0] = p[v[0]];
	m_componentTriangle.x()[1] = p[v[1]];
	m_componentTriangle.x()[2] = p[v[2]];
	
	return gjk::Intersect1<gjk::TriangleSet, BoundingBox>::Evaluate(m_componentTriangle, box);
}

void ATriangleMesh::reverseTriangleNormals()
{
	const unsigned n = numTriangles();
	for(unsigned i=0;i<n;++i) {
		unsigned * v = triangleIndices(i);
		unsigned t = v[0];
		v[0] = v[2];
		v[2] = t;
	}
}

void ATriangleMesh::calculateVertexNormals()
{
    const int nv = numPoints();
    Vector3F * ndst = vertexNormals();
    for(int i=0;i<nv;++i) {
        ndst[i].set(0.f, 0.f, 0.f);
    }
    
    const unsigned * inds = indices();
    const unsigned ntris = numTriangles();
    for(unsigned i=0;i<ntris;++i) {
        const Vector3F triNm = triangleNormal(i);
        const unsigned * triInd = triangleIndices(i);
        ndst[triInd[0]] += triNm;
        ndst[triInd[1]] += triNm;
        ndst[triInd[2]] += triNm;
    }
    
    for(int i=0;i<nv;++i) {
        ndst[i].normalize();
    }
    
}

void ATriangleMesh::setTriangleTexcoord(const int & idx, const Float2 * uvs)
{
	m_texcoord[idx * 3] = uvs[0];
	m_texcoord[idx * 3 + 1] = uvs[1];
	m_texcoord[idx * 3 + 2] = uvs[2];
}
	
const Float2 * ATriangleMesh::triangleTexcoord(const int & idx) const
{ return &m_texcoord[idx * 3]; }

float * ATriangleMesh::triangleTexcoords()
{ return (float *)m_texcoord.get(); }

const float * ATriangleMesh::triangleTexcoords() const
{ return (float *)m_texcoord.get(); }

ATriangleMesh* ATriangleMesh::CreateFromData(const int& np,
		const int& nt,
		const int* triind,
		const float * vertpos,
		const float * vertnml,
		const float * vertcol,
		const float * tritexcoord)
{
	ATriangleMesh* msh = new ATriangleMesh;
	msh->create(np, nt);
	msh->createVertexColors(np);
	unsigned * indDst = msh->indices();
	memcpy(indDst, triind, nt * 12);
	Vector3F * pntDst = msh->points();
	memcpy(pntDst, vertpos, np * 12);
	Vector3F * nmlDst = msh->vertexNormals();
	memcpy(nmlDst, vertnml, np * 12);
	float * colDst = msh->vertexColors();
	memcpy(colDst, vertcol, np * 12);
	float * texcoordDst = msh->triangleTexcoords();
	memcpy(texcoordDst, tritexcoord, nt * 24);
	
	return msh;
}

}
//:~