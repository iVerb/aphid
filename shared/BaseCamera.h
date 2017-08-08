/*
 *  BaseCamera.h
 *  lbm3d
 *
 *  Created by jian zhang on 7/25/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include <math/Matrix44F.h>

namespace aphid {

class BoundingBox;

class BaseCamera {
public:
	BaseCamera();
	virtual ~BaseCamera();
	
	virtual bool isOrthographic() const;
	void reset(const Vector3F & pos);
	void lookFromTo(Vector3F & from, Vector3F & to);
	void setPortWidth(unsigned w);
	void setPortHeight(unsigned h);
	void setHorizontalAperture(float w);
	void updateInverseSpace();
	void getMatrix(float* m) const;
	void tumble(int x, int y);
	void track(int x, int y);
	virtual void zoom(int y);
	void moveForward(int y);
	
	char screenToWorldPoint(int x, int y, Vector3F & worldPos) const;
	void screenToWorldVector(int x, int y, Vector3F & worldVec) const;
	virtual void screenToWorldVectorAt(int x, int y, float depth, Vector3F & worldVec) const;
/// origin on near clipping plane
	virtual void incidentRay(int x, int y, Vector3F & origin, Vector3F & worldVec) const;
	Vector3F eyePosition() const;
	Vector3F eyeDirection() const;
/// width / height
	float aspectRatio() const;
	float nearClipPlane() const;
	float farClipPlane() const;
	
	virtual float fieldOfView() const;
	virtual float frameWidth() const;
	virtual float frameHeight() const;
	virtual float frameWidthRel() const;
	float getHorizontalAperture() const;
	virtual void frameCorners(Vector3F & bottomLeft, Vector3F & bottomRight, Vector3F & topRight, Vector3F & topLeft) const;
	void copyTransformFrom(BaseCamera * another);
/// pixel origin is left-top
/// right-top is 0.5,0.5
/// left-bottom is -0.5,-0.5
	void getScreenCoord(float& cx, float& cy,
			const int& px, const int& py) const;
	
	void setNearClipPlane(float x);
	void setFarClipPlane(float x);
	virtual void setFieldOfView(float x);
	
	void traverse(const Vector3F & v);
	void frameAll(const BoundingBox & b);
	void setViewTransform(const Matrix44F & mat,
				const float & focalLength);
	
	Matrix44F fSpace, fInverseSpace;
	Vector3F fCenterOfInterest;
	unsigned fPortWidth, fPortHeight;
	float fHorizontalAperture;
	float m_nearClipPlane, m_farClipPlane;
};

}