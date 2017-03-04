/*
 *  ExampleWorks.h
 *  proxyPaint
 *
 *  select viz
 *  query and edit example parameters
 *
 *  Created by jian zhang on 3/1/17.
 *  Copyright 2017 __MyCompanyName__. All rights reserved.
 *
 */
 
 #ifndef RHI_EXAMPLE_WORKS_H
 #define RHI_EXAMPLE_WORKS_H
 
 #include "ProxyVizNode.h"
 
 class MSelectionList;
 class MObjectArray;
 
 class ExampleWorks {
 
 public:
	ExampleWorks();
	virtual ~ExampleWorks();
	
	MString getExampleStatusStr();
	float getShowVoxelThreshold();
	
/// change status of idx-th example
/// visible=0;active=true;color=.5 .5 .5
	bool setExampleStatus(int idx, const std::string & expression);
	
 protected:
 /// active viz
	static aphid::ProxyViz * PtrViz;
	static MObject ObjViz;
	
	bool validateViz(const MSelectionList &sels);
	bool validateSelection();
	
	void processShowVoxelThreshold(float x);
	
 private:
	void getConnectExamples(MObjectArray & exmpOs);
	void addBoolStatusStrSeg(MString & res, bool b, const char * segName);
	void addVec3StatusStrSeg(MString & res, double r, double g, double b, const char * segName);
	void setExampleStatus(const MObject & exmpO,
				const std::string & expression);
	void setExampleBoolAttrib(const MObject & exmpO,
				const MString & attribName,
				const std::string & expression);
	void setExampleCol3Attrib(const MObject & exmpO,
				const MString & attribName,
				const std::string & expression);
	bool matchedBool(bool & val,
				const std::string & expression);
	bool matchedVec3(float * vs,
				const std::string & expression);
	
 };
 
 #endif
 
