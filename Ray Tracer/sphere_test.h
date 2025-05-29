#ifndef SPHERE_TEST_H
#define SPHERE_TEST_H

#include "Vec3.h"
#include "Ray.h"
#include<cmath>

//Here we assume there is a sphere located at C with radius R in the scene.
//The intersection of a ray: R = Q + td, where Q is otigin of the ray, t is parameter, and d is direction of the ray
//with the circle with center at location C will depend upon the quadratioc equation of t.
//If atleast one of the roots of this quadratic equation exist then we can say that the ray R is intersecting with the sphere.
//In the quadratic equation a = d.d ; b = -2d.(C-Q) ; c = (C-Q).(C-Q) - r*r.

//This function only do intersection detection.
bool g_hitSphere(const Point3& center, double radius, const Ray& r) {
	Vec3 oc = center - r.m_getOrigin();
	auto a = g_dot(r.m_getDirection(), r.m_getDirection());
	auto b = -2.0 * g_dot(r.m_getDirection(), oc);
	auto c = g_dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	return (discriminant >= 0);
}

//This function returns value of parameter t on ray intersection with sphere.
double g_hitSphere2(const Point3& center, double radius, const Ray& r) {
	Vec3 oc = center - r.m_getOrigin();
	auto a = g_dot(r.m_getDirection(), r.m_getDirection());
	auto b = -2.0 * g_dot(r.m_getDirection(), oc);
	auto c = g_dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	
	if (discriminant < 0) {
		return -1;
	}
	else {
		return (-b - std::sqrt(discriminant)) / (2.0 * a); //We need minimum t that's why t = (b-sqrt(discriminant))/2a is taken
	}
}

//This function returns value of parameter t on ray intersection with sphere. This is optimized one
double g_hitSphere2Optimized(const Point3& center, double radius, const Ray& r) {
	Vec3 oc = center - r.m_getOrigin();
	auto a = r.m_getDirection().m_lengthSquared();
	auto h = g_dot(r.m_getDirection(), oc); //b = -2h
	auto c =oc.m_lengthSquared() - radius * radius;
	auto discriminant =h*h - a * c;

	if (discriminant < 0) {
		return -1;
	}
	else {
		return (h - std::sqrt(discriminant)) / a;
	}
}

#endif