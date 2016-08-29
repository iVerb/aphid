/*
 *  LegendreTest.h
 *  foo
 *
 *  Created by jian zhang on 7/14/16.
 *  Copyright 2016 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"

namespace ttg {

class LegendreTest : public Scene {

#define M_NUM_EVAL 257  /// m, the number of evaluation points
#define POLY_MAX_DEG 8  /// n, the highest order polynomial to evaluate
#define INTERVAL_A -1.f
#define INTERVAL_B  1.f
#define DX_SAMPLE 0.0078125f

/// each evaluate has 0 - m th polynomial
	float m_v[M_NUM_EVAL * (POLY_MAX_DEG+1)];
/// integral of pi(x)pi(x)
	float m_pipi[POLY_MAX_DEG+1];
		
	float m_exactEvaluate[M_NUM_EVAL];
	float m_approximateEvaluate[M_NUM_EVAL];

	float m_coeff[POLY_MAX_DEG+1];
	
/// integral of xpi(x)pi(x)
	float m_xpipi[POLY_MAX_DEG+1];
	
public:
	LegendreTest();
	virtual ~LegendreTest();
	
	virtual const char * titleStr() const;
	virtual bool init();
	virtual void draw(aphid::GeoDrawer * dr);
	
private:
/// integral <pi, pi>
	void computePipi();
	
	float evaluateExact(const float & x) const;
	float discreteIntegral(int n, const float * y, const float * v) const;
	void printValues(const char * h, int n, const float * y) const;
	void drawEvaluate(const float * y, aphid::GeoDrawer * dr);
	void drawLegendrePoly(int m, aphid::GeoDrawer * dr);
/// m num of y samples
/// n max degree of evaluate
/// y f(x) in m samples
/// x sample points
/// pipi <pi, pi>
/// v polynomial values
/// Px buffer for Pi(x) samples
	void computeCoeff(float * coeff, int n, const float * y,
						const float * pipi) const;
	void computeApproximated(float * yhat, int n, const float * coeff) const;
	
/// Pi(x) sample pre-ccomputed i-th polynomial
	float samplePolyValue(int i, float x) const;
	
};

}