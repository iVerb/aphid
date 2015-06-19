/*
 *  HTriangleMesh.cpp
 *  testbcc
 *
 *  Created by jian zhang on 4/23/15.
 *  Copyright 2015 __MyCompanyName__. All rights reserved.
 *
 */

#include "HTriangleMesh.h"
#include <AllHdf.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <CurveGroup.h>
#include <BaseBuffer.h>
#include <ATriangleMesh.h>

HTriangleMesh::HTriangleMesh(const std::string & path) : HBase(path) 
{
}

HTriangleMesh::~HTriangleMesh() {}

char HTriangleMesh::verifyType()
{
	if(!hasNamedAttr(".ntri"))
		return 0;

	if(!hasNamedAttr(".nv"))
		return 0;
		
	if(!hasNamedAttr(".dag"))
		return 0;
	
	return 1;
}

char HTriangleMesh::save(ATriangleMesh * tri)
{
	int nv = tri->numPoints();
	if(!hasNamedAttr(".nv"))
		addIntAttr(".nv");
	
	writeIntAttr(".nv", &nv);
	
	int nt = tri->numTriangles();
	if(!hasNamedAttr(".ntri"))
		addIntAttr(".ntri");
	
	writeIntAttr(".ntri", &nt);
	
	if(!hasNamedData(".p"))
	    addVector3Data(".p", nv);
	
	writeVector3Data(".p", nv, (Vector3F *)tri->points());
	
	if(!hasNamedData(".a"))
	    addIntData(".a", nv);
	
	writeIntData(".a", nv, (int *)tri->anchors());
		
	if(!hasNamedData(".v"))
	    addIntData(".v", nt * 3);
	
	writeIntData(".v", nt * 3, (int *)tri->indices());
	
	if(!hasNamedAttr(".dag"))
		addStringAttr(".dag", tri->dagName().size());
		
	std::cout<<" dag name is "<<tri->dagName();
	writeStringAttr(".dag", tri->dagName());

	return 1;
}

char HTriangleMesh::load(ATriangleMesh * tri)
{
	if(!verifyType()) return false;
	int nv = 3;
	
	readIntAttr(".nv", &nv);
	
	int nt = 1;
	
	readIntAttr(".ntri", &nt);
	
	std::string dagName;
	readStringAttr(".dag", dagName);
	tri->setDagName(dagName);
	
	tri->create(nv, nt);
	
	readVector3Data(".p", nv, (Vector3F *)tri->points());
	readIntData(".a", nv, (unsigned *)tri->anchors());
	readIntData(".v", nt * 3, (unsigned *)tri->indices());
	
	return 1;
}
//:~