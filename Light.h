#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "defs.h"

using namespace std;

// Class for point lights
class PointLight
{
public:	
    Vector3f position;	// Position of the point light

    PointLight(const Vector3f & position, const Vector3f & intensity);	// Constructor
    Vector3f computeLightContribution(const Vector3f& p); // Compute the contribution of light at point p

private:

    Vector3f intensity;	// Intensity of the point light

    Vector3f scalarMultiplication(float t, Vector3f direction) const;
	Vector3f vectorSubtraction(Vector3f a, Vector3f b) const;
	Vector3f vectorAddition(Vector3f a, Vector3f b) const;
	float dotProduct(Vector3f a, Vector3f b) const;
	float determinant(Vector3f a, Vector3f b, Vector3f c) const;
	Vector3f normalize( Vector3f v) const;
	Vector3f normal( Vector3f center, Vector3f intersectionPoint) const;
};


#endif
