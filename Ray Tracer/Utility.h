#ifndef UTILITY_H
#define UTILITY_H

#include<cmath>
#include<cstdlib>
#include<random>
#include<iostream>
#include<limits> //specifies the storage limits of various integer based data types, specific to the system, in terms of macro constants
#include<memory>

//C++ std usings:
using std::make_shared;
using std::shared_ptr;

//Constants:
const double infinity = std::numeric_limits<double>::infinity(); //std::numeric_limits<> is a class declared in limit.h
const double pi = 3.1415926535897932385;

//Utility functions:
inline double g_degreesToRadians(double degrees) {

	return degrees * pi / 180;
}

inline double g_randomDouble() {
	//Returns a random real in [0,1)

	/*static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);*/
	return std::rand() / (RAND_MAX + 1.0);
}

inline double g_randomDouble(double min, double max) {
	//Returns a random real in [min,max)
	return min + (max - min)*g_randomDouble();
}

inline int g_randomInt(int min, int max) {
	//Returns a random integer in [min, max]
	return int(g_randomDouble(min, max + 1));
}

//Common headers:
#include "Color.h"
#include "Interval.h"
#include "Ray.h"
#include "Vec3.h"

#endif