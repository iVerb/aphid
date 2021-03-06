/*
 *  ShrubWorks.cpp
 *  
 *
 *  Created by jian zhang on 12/26/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */

#include "ShrubWorks.h"
#include <maya/MSelectionList.h>
#include <maya/MItMeshVertex.h>
#include <maya/MDagModifier.h>
#include <mama/AHelper.h>
#include <mama/ASearchHelper.h>
#include <mama/MeshHelper.h>
#include <mama/ConnectionHelper.h>
#include <mama/SelectionHelper.h>
#include "ShrubVizNode.h"
#include "ExampVizNode.h"
#include <gpr/PCASimilarity.h>
#include <gpr/PCAFeature.h>
#include <sdb/Types.h>

namespace aphid {

ShrubWorks::ShrubWorks()
{}

ShrubWorks::~ShrubWorks()
{}

void ShrubWorks::getMeshVertices(DenseMatrix<float> * vertices, 
					int & iRow, 
					BoundingBox & bbox, 
					const MDagPath & meshPath, 
					const MDagPath & transformPath) const
{
	AHelper::Info<MString>(" get mesh vertices", meshPath.fullPathName() );
	
	MMatrix worldTm = AHelper::GetWorldParentTransformMatrix2(meshPath, transformPath);
	
	Vector3F fpnt;
	MStatus stat;
	MItMeshVertex vertIt(meshPath, MObject::kNullObj, &stat);
	if(!stat) {
		AHelper::Info<MString >(" ShrubWorks getMeshVertices cannot it mesh vertex", meshPath.fullPathName() );
		return;
	}
	
	for(;!vertIt.isDone();vertIt.next() ) {
		
		MPoint pnt = vertIt.position();
		
		pnt *= worldTm;
		
		fpnt.set(pnt.x, pnt.y, pnt.z);
		
		vertices->copyRow(iRow, (const float *)&fpnt);
		
		iRow++;
		
		bbox.expandBy(fpnt);
	}
	
}

int ShrubWorks::getGroupMeshVertices(DenseMatrix<float> * vertices,
					BoundingBox & bbox, 
					const MDagPath & path) const
{
	AHelper::Info<const char *>(" ShrubWorks get world mesh vertices in group", path.fullPathName().asChar() );
	MDagPathArray meshPaths;
	ASearchHelper::LsAllTypedPaths(meshPaths, path, MFn::kMesh);

	const int n = meshPaths.length();
	if(n < 1) {
		AHelper::Info<MString>(" WARNING ShrubWorks find no mesh in group", path.fullPathName() );
		return 0;
	}
	
	int nv = 0;
	for(int i=0;i<n;++i) {
		MeshHelper::CountMeshNv(nv, meshPaths[i]);
	}
	
	AHelper::Info<int>(" group mesh n v", nv );
		
	vertices->resize(nv, 3);
	
	int iRow = 0;
	for(int i=0;i<n;++i) {
		getMeshVertices(vertices, iRow, bbox, meshPaths[i], path);
	}
	
	AHelper::Info<BoundingBox>(" group mesh box", bbox );
	
	return nv;
}

void ShrubWorks::addSimilarity(std::vector<SimilarityType * > & similarities, 
					const DenseMatrix<float> & vertices,
					const int & groupi) const
{
	SimilarityType * asim = new SimilarityType();
	similarities.push_back(asim);
	*asim->t1 = groupi;
	asim->t2->begin(vertices, 2);
}

bool ShrubWorks::findSimilar(std::vector<SimilarityType * > & similarities, 
					const DenseMatrix<float> & vertices) const
{
	const int n = similarities.size();
	if(n<1) {
		return false;
	}
	
	for(int i=0;i<n;++i) {
		if(similarities[i]->t2->select(vertices, 2) ) {
			return true;
		}
	}
	return false;
}

void ShrubWorks::clearSimilarity(std::vector<SimilarityType * > & similarities) const
{
	const int n = similarities.size();
	if(n<1) {
		return;
	}
	
	for(int i=0;i<n;++i) {
		delete similarities[i]->t1;
		delete similarities[i]->t2;
	}
	similarities.clear();
}

void ShrubWorks::scaleSpace(DenseMatrix<float> & space,
					const float * a,
					const float * b) const
{
	for(int i=0;i<3;++i) {
		float r = 1.f;
		if(b[3+i] > 1.0e-3f) {
			r = a[3+i] / b[3+i];
		}
		
		float * c = space.column(i);
		Vector3F v(c);
		v.normalize();
		c[0] = v.x * r;
		c[1] = v.y * r;
		c[2] = v.z * r;
	}
}

int ShrubWorks::countExamples(const std::vector<SimilarityType * > & similarities,
								FeatureExampleMap & ind) const
{
	const int ns = similarities.size();
	for(int i=0;i<ns;++i) {
		AHelper::Info<int>(" separate similarity", i);
/// K = 4 by default
		similarities[i]->t2->separateFeatures();
		
		const int & ne = similarities[i]->t2->numGroups();
		AHelper::Info<int>(" into n example", ne );
		
		for(int j=0;j<ne;++j) {
			const int ej = similarities[i]->t2->bestFeatureInGroup(j);
			ind[(i<<8) | j] = Int2(ej, -1);
		}
	}
	
	int c = 0;
	FeatureExampleMap::iterator it = ind.begin();
	for(;it != ind.end();++it) {
		it->second.y = c;
		c++; 
	}
	return c;
}

void ShrubWorks::addInstances(const MObject & vizNode,
							const std::vector<SimilarityType * > & similarities,
							 FeatureExampleMap & exampleGroupInd) const
{
	ShrubVizNode * viz = (ShrubVizNode *)MFnDependencyNode(vizNode).userNode();
	viz->clearInstances();
	
	const int ns = similarities.size();
	DenseMatrix<float> transFeature(4, 4);
	transFeature.setIdentity();
	BoundingBox boxFeature, boxExample;
	int grpFeature = 0;
	
	for(int i=0;i<ns;++i) {
		const int & nf = similarities[i]->t2->numFeatures();
		for(int j=0;j<nf;++j) {
			const TSimilarity * simi = similarities[i]->t2;
			
			simi->getFeatureGroup(grpFeature, j);
			simi->getFeatureSpace(transFeature.raw(), j);
			simi->getFeatureBound((float *)&boxFeature, j, 1);
			
			const Int2 groupI = exampleGroupInd[(i<<8) | grpFeature];
			
/// best local example in similarity
			simi->getFeatureBound((float *)&boxExample, groupI.x, 1);
			scaleSpace(transFeature, (const float *)&boxFeature, (const float *)&boxExample );
			
/// global example in viz
			viz->addInstance(transFeature, groupI.y);
		}
	}
}

void ShrubWorks::addSimilarities(std::vector<SimilarityType * > & similarities,
					BoundingBox & totalBox,
					const MDagPathArray & paths) const
{
	const int ngrp = paths.length();
	for(int i=0;i<ngrp;++i) {
		BoundingBox grpBox;
		DenseMatrix<float> grpVertD;
		const int nv = getGroupMeshVertices(&grpVertD, grpBox, paths[i]);
		if(nv < 10 ) {
			continue;
		}
		
		if(!findSimilar(similarities, grpVertD)) {
			addSimilarity(similarities, grpVertD, i);
		}
		
		totalBox.expandBy(grpBox);
	}
}

void ShrubWorks::addExamples(const MObject & vizNode,
					const std::vector<SimilarityType * > & similarities,
					FeatureExampleMap & exampleGroupInd,
					const MDagPathArray & paths) const
{
    ShrubVizNode * viz = (ShrubVizNode *)MFnDependencyNode(vizNode).userNode();
    viz->clearExamples();
    viz->disableExampleInput();
	
	int nps = 0;
	const int ns = similarities.size();
	for(int i=0;i<ns;++i) {
		const int & ne = similarities[i]->t2->numGroups();
		
		MIntArray tris;
		MeshHelper::GetMeshTrianglesInGroup(tris, paths[*similarities[i]->t1]);
		
		for(int j=0;j<ne;++j) {
			const Int2 groupI = exampleGroupInd[(i<<8) | j];
			AHelper::Info<int>("loc grp", groupI.x);
			AHelper::Info<int>("glb grp", groupI.y);
			
			DenseMatrix<float> pnts;
			similarities[i]->t2->getFeaturePoints(pnts, groupI.x, 1);
			BoundingBox bbox;
			similarities[i]->t2->getFeatureBound((float *)&bbox, groupI.x, 1);
			
			MObject exampleNode = ConnectionHelper::GetConnectedNode(vizNode, "inExample", nps);
			bool toConnect = false;
			if(exampleNode == MObject::kNullObj) {
				exampleNode = AHelper::CreateDagNode("proxyExample", "proxyExample");
				toConnect = true;
			}
			ExampViz * example = (ExampViz *)MFnDependencyNode(exampleNode).userNode();
			example->voxelize3(pnts, tris, bbox);
			
			if(toConnect) {
				ConnectionHelper::ConnectToArray(exampleNode, "outValue",
							vizNode, "inExample");
			}
			
			viz->addAExample(example);
							
			nps++;
		}
	}
	
	viz->enableExampleInput();
	
/// evaluate input examples
	MPlug exmPlug;
	MObject nouse;
	AHelper::getNamedPlug(exmPlug, vizNode, "inExample" );
	for(int i=0;i<exmPlug.numElements();++i) {
		MPlug anexm = exmPlug.elementByLogicalIndex(i);
		anexm.getValue(nouse);
	}
}

MStatus ShrubWorks::creatShrub()
{
	MSelectionList selList;
	MGlobal::getActiveSelectionList ( selList );
	if(selList.length() < 1) {
		MGlobal::displayWarning(" ERROR ShrubWorks empty selection, select groups to create shrub");
		return MS::kSuccess;
	}
	
	MDagPathArray paths;
    MItSelectionList iter( selList );
	for ( ; !iter.isDone(); iter.next() ) {								
		MDagPath apath;		
		iter.getDagPath( apath );
        paths.append(apath);
	}
	
	const int ngrp = paths.length();
	if(ngrp < 1) {
		AHelper::Info<int>(" ERROR ShrubWorks found no mesh in selected groups", ngrp );
		return MS::kSuccess;
	}
	
	AHelper::Info<int>(" ShrubWorks select n groups", ngrp );
	
	std::vector<SimilarityType * > similarities;
	BoundingBox totalBox;
	
	addSimilarities(similarities, totalBox, paths);
	
	MObject shrubNode = SelectionHelper::GetTypedNode(selList, "shrubViz", MFn::kPluginLocatorNode);
	if(shrubNode == MObject::kNullObj) {
		shrubNode = AHelper::CreateDagNode("shrubViz", "shrubViz");
	}
	ShrubVizNode * viz = (ShrubVizNode *)MFnDependencyNode(shrubNode).userNode();
	viz->setBBox(totalBox);
	
	const int ns = similarities.size();
	AHelper::Info<int>(" found n similariy", ns );
	AHelper::Info<BoundingBox>(" total bbox", totalBox );
	
	FeatureExampleMap exampleGroupInd;
	
	int totalNe = countExamples(similarities, exampleGroupInd);
	AHelper::Info<int>(" total n example", totalNe );
	
	addExamples(shrubNode, similarities, exampleGroupInd, paths);

	addInstances(shrubNode, similarities, exampleGroupInd);
	
	clearSimilarity(similarities);
	exampleGroupInd.clear();
	return MS::kSuccess;
}

}