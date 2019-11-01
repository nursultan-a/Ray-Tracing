#include "Light.h"

#include <math.h>
#include <limits>

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    Vector3f wi = vectorSubtraction(position, p);
    float R = dotProduct(wi, wi);

    return scalarMultiplication((1/(R*R)),intensity); 

    //return intensity over r square
}

Vector3f PointLight::normal( Vector3f a, Vector3f b) const{
    Vector3f result;

    result.x = a.y * b.z - a.z*b.y;
    result.y = a.z * b.x - a.x*b.z;
    result.z = a.x * b.y - a.y*b.x;

    return result;
}
Vector3f PointLight::normalize( Vector3f v) const{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (len != 0.0)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}
float PointLight::determinant(Vector3f a, Vector3f b, Vector3f c) const{
    return a.x*b.y*c.z + b.x*c.y*a.z + c.x*a.y*b.z - a.z*b.y*c.x - b.z*c.y*a.x - c.z*a.y*b.x;
}
Vector3f PointLight::scalarMultiplication(float t, Vector3f direction) const{
    Vector3f result;

    result.x = direction.x * t;
    result.y = direction.y * t;
    result.z = direction.z * t;

    return result;
}

Vector3f PointLight::vectorSubtraction(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}
Vector3f PointLight::vectorAddition(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;

}

float PointLight::dotProduct(Vector3f a, Vector3f b) const{
    
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
