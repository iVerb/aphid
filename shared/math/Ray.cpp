/*
 *  Ray.cpp
 *  lapl
 *
 *  Created by jian zhang on 3/16/13.
 *  Copyright 2013 __MyCompanyName__. All rights reserved.
 *
 */

#include "Ray.h"
#include <iostream>
#include "line_math.h"

namespace aphid {

Ray::Ray() {}

Ray::Ray(const Vector3F& pfrom, const Vector3F& vdir, float tmin, float tmax)
{
	m_origin = pfrom;
	m_dir = vdir;
	m_tmin = tmin;
	m_tmax = tmax;
	if(m_tmax > 1e7f) {
		std::cout<<"\n truncate ray max to 1e7f";
		m_tmax = 1e7f;
	}
}

Ray::Ray(const Vector3F& pfrom, const Vector3F& pto) 
{
	m_origin = pfrom;
	m_dir = pto - pfrom;
	m_tmin = 0.f;
	m_tmax = m_dir.length();
	if(m_tmax > 1e7f) {
		std::cout<<"\n truncate ray max to 1e7f";
		m_tmax = 1e7f;
	}
	m_dir.normalize();
}

void Ray::operator=(const Ray& b)
{ 
	m_origin = b.m_origin;
	m_tmin = b.m_tmin;
	m_dir = b.m_dir;
	m_tmax = b.m_tmax;
}

Vector3F Ray::travel(const float & t) const
{ return m_origin + m_dir * t; }

Vector3F Ray::destination() const
{ return m_origin + m_dir * m_tmax; }

const Vector3F Ray::closestPointOnRay(const Vector3F & p, float * t) const
{
	float tt = (p - m_origin).dot(m_dir);
	if(t) *t = tt;
	return m_origin + m_dir * tt;
}

const Vector3F & Ray::origin() const
{ return m_origin; }

const std::string Ray::str() const
{
	std::stringstream sst;
	sst.str("");
    sst<<"("<<m_origin<<","<<destination()<<")";
	return sst.str();
}

float Ray::length() const
{ return m_tmax - m_tmin; }

float Ray::projectP(const Vector3F & q, Vector3F & pproj) const
{ 
	Vector3F voq = q - m_origin;
	float voqdotdir = voq.dot(m_dir);
	if(voqdotdir < m_tmin) {
		pproj = travel(m_tmin);
		return -1.f;
	}
		
	if(voqdotdir >= m_tmax) {
		pproj = travel(m_tmax);
		return -1.f;
	}
		
	pproj = travel(voqdotdir);
	return voqdotdir;
	
}

float Ray::distanceToPoint(const Vector3F & q) const
{
	Vector3F pol;
	projectP(q, pol);
	return q.distanceTo(pol);
}

Beam::Beam() {}

Beam::Beam(const Vector3F& pfrom, const Vector3F& pto,
		const float & rmin, const float & rmax)
{
	m_ray = Ray(pfrom, pto);
	m_rmin = rmin; m_rmax = rmax; 
	drdt = (rmax - rmin) / (m_ray.m_tmax - m_ray.m_tmin);
}

Beam::Beam(const Ray & r, const float & rmin, const float & rmax)
{ 
	m_ray = r; m_rmin = rmin; m_rmax = rmax; 
	drdt = (rmax - rmin) / (r.m_tmax - r.m_tmin);
}

const Ray & Beam::ray() const
{ return m_ray; }

const float & Beam::tmin() const
{ return m_ray.m_tmin; }

const float & Beam::tmax() const
{ return m_ray.m_tmax; }

float Beam::radiusAt(const float & t) const
{ return m_rmin + drdt * (t - m_ray.m_tmin); }

void Beam::setLength(const float & tmin, const float & tmax)
{
	m_rmin += (tmin - m_ray.m_tmin) * drdt;
	m_ray.m_tmin = tmin;
	m_rmax += (tmax - m_ray.m_tmax) * drdt;
	m_ray.m_tmax = tmax;
}

void Beam::setTmin(const float & x)
{
	m_rmin += (x - m_ray.m_tmin) * drdt;
	m_ray.m_tmin = x;
}

void Beam::setTmax(const float & x)
{
	m_rmax += (x - m_ray.m_tmax) * drdt;
	m_ray.m_tmax = x;
}

float Beam::length() const
{ return m_ray.length(); }

const Vector3F & Beam::origin() const
{ return m_ray.origin(); }

Vector3F Beam::destination() const
{ return m_ray.destination(); }

float Beam::projectP(const Vector3F & q, Vector3F & pproj) const
{ return m_ray.projectP(q, pproj); }

}