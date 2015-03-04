#ifndef _GJK_MATH_H_
#define _GJK_MATH_H_

#include "bvh_math.cu"
#include "matrix_math.cu"

typedef float4 BarycentricCoordinate;

struct Simplex {
    float3 p[4];
	float3 pA[4];
	float3 pB[4];
    int dimension;
};

struct TetrahedronProxy {
    float3 p[4];
    float3 v[4];
};

inline __device__ int isTriangleDegenerate(float3 a, float3 b, float3 c)
{
    float3 ab = float3_difference(b, a);
    float3 ac = float3_difference(c, a);
    float3 nor = float3_cross(ab, ac);
    return (float3_length2(nor) < 1e-6);
}

inline __device__ int isTetrahedronDegenerate(float3 a, float3 b, float3 c, float3 d)
{
    mat44 m;
    fill_mat44(m, a, b, c, d);
    
    float D0 = determinant44(m) ;
    if(D0 < 0.f) D0 = -D0;
    return (D0 < 0.01f);
}

inline __device__ void resetSimplex(Simplex & s)
{ s.dimension = 0; }

inline __device__ void addToSimplex(Simplex & s, float3 p, float3 localA, float3 localB)
{
    if(s.dimension < 1) {
        s.p[0] = p;
        s.pA[0] = localA;
        s.pB[0] = localB;
        s.dimension = 1;
    }
    else if(s.dimension < 2) {
		if(distance2_between(p, s.p[0]) > 1e-8) {
		    s.p[1] = p;
		    s.pA[1] = localA;
		    s.pB[1] = localB;
		    s.dimension = 2;
		}
    }
    else if(s.dimension < 3) {
		if(!isTriangleDegenerate(s.p[0], s.p[1], p)) {
		    s.p[2] = p;
		    s.pA[2] = localA;
		    s.pB[2] = localB;
		    s.dimension = 3;
		}
    }
    else {
        if(!isTetrahedronDegenerate(s.p[0], s.p[1], s.p[2], p)) {
		    s.p[3] = p;
		    s.pA[3] = localA;
		    s.pB[2] = localB;
		    s.dimension = 4;
		}
    }
}

inline __device__ float3 initialPoint(TetrahedronProxy tet, float3 ref)
{
    float3 r = float3_difference(tet.p[0], ref);
    
    if(float3_length2(r) < 1e-6)
        r = float3_difference(tet.p[1], ref);
    
    return r;
}

inline __device__ float3 supportPoint(TetrahedronProxy tet, float3 ref, float margin, float3 & localP)
{
    float maxDotv = -1e8;
    float dotv;
    
    float3 dMargin = scale_float3_by(float3_normalize(ref), margin);
    float3 res, wp;
    
    int i;
    
    float3 center = make_float3(0.f, 0.f, 0.f);
    for(i=0; i<4; i++) {
        center = float3_add(tet.p[i], center);
    }
    center = scale_float3_by(center,0.25f);
    
    for(i=0; i<4; i++) {
        wp = float3_add(tet.p[i], dMargin);
        dotv = float3_dot(wp, ref);
        if(dotv > maxDotv) {
            maxDotv = dotv;
            res = wp;
            localP = float3_difference(tet.p[i], center);
        }
    }
    
    return res;
}

inline __device__ BarycentricCoordinate getBarycentricCoordinate4(float3 p, float3 * v)
{
    BarycentricCoordinate coord;
    
    mat44 m;
    fill_mat44(m, v[0], v[1], v[2], v[3]);
    
    float D0 = determinant44(m);
    if(D0 == 0.f) {
        coord.x = coord.y = coord.z = coord.w = -1.f;
        return coord;
    }  
    
    fill_mat44(m, p, v[1], v[2], v[3]);
    coord.x = determinant44(m) / D0;
    fill_mat44(m, v[0], p, v[2], v[3]);
    coord.y = determinant44(m) / D0;
    fill_mat44(m, v[0], v[1], p, v[3]);
    coord.z = determinant44(m) / D0;
    fill_mat44(m, v[0], v[1], v[2], p);
    coord.w = determinant44(m) / D0;
    
    return coord;
}

inline __device__ int pointInsideTetrahedronTest(float3 p, float3 * tet)
{
    BarycentricCoordinate coord = getBarycentricCoordinate4(p, tet);
    return (coord.x >=0 && coord.x <=1 && 
        coord.y >=0 && coord.y <=1 &&
        coord.z >=0 && coord.z <=1 &&
        coord.w >=0 && coord.w <=1);
}

inline __device__ int isPointInsideSimplex(Simplex & s, float3 p)
{
    if(s.dimension > 3) {
        return pointInsideTetrahedronTest(p, s.p);
    }
    return 0;
}

inline __device__ void computeSeparateDistance(Simplex & s, float3 Pref, 
                                               TetrahedronProxy prxA,
                                               TetrahedronProxy prxB)
{
	float3 v = initialPoint(prxA, Pref);
	
	float3 w, supportA, supportB, localA, localB;
	float margin = 0.05f;
	float v2;
	int i = 0;

	while(i<99) {
	    supportA = supportPoint(prxA, float3_reverse(v), margin, localA);
	    supportB = supportPoint(prxB, v, margin, localB);
	    
	    w = float3_difference(supportA, supportB);
	    
	    v2 = float3_length2(v);
	    if((v2 - float3_dot(w, v)) < 0.0001f * v2) {
	        return;
	    }
	    
	    addToSimplex(s, w, localA, localB);
	    
	    if(isPointInsideSimplex(s, Pref)) {
	        return;
	    }
	    
	    i++;
	}
}

#endif        //  #ifndef _GJK_MATH_H_

