#include "Camera.h"



// not default
#include <string.h>
#include <math.h> 

//not dependend
#include <iostream>

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    this->id = id;

    strcpy(this->imageName, imageName);

    this->imgPlane.left = imgPlane.left;
    this->imgPlane.right = imgPlane.right;
    this->imgPlane.bottom = imgPlane.bottom;
    this->imgPlane.top = imgPlane.top;
    this->imgPlane.distance = imgPlane.distance;
    this->imgPlane.nx = imgPlane.nx;
    this->imgPlane.ny = imgPlane.ny;

    // Camera position according to global/world coordinate system
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;

    // camera orientation: v->up, w-> opposite direction of the cameras lens/gaze, u-> cross product of v and u 
    v.x = up.x;
    v.y = up.y;
    v.z = up.z;

    this->gaze.x = gaze.x;
    this->gaze.y = gaze.y;
    this->gaze.z = gaze.z;
    

    w.x = (-1)*gaze.x;
    w.y = (-1)*gaze.y;
    w.z = (-1)*gaze.z;

    u = crossProduct(v,w);

    // camera orientation vectors should be normalized
    w = normalize(w);
    u = normalize(u);
    v = normalize(v);

    // center/middle point of image plane
    midPoint = vectorAddition(position, scalarMultiplication(imgPlane.distance, gaze));

    // vector that points from Camera positioin to (0,0) pixel of image plane
    q =  vectorAddition(midPoint,vectorAddition(scalarMultiplication(imgPlane.left, u), scalarMultiplication(imgPlane.top, v)));

    std::cout << "distance: " << imgPlane.distance << std::endl;
    std::cout << "camera position: [" << position.x << "," << position.y << "," << position.z << "]" << std::endl;
    std::cout << "camera orientation:"<< std::endl;
    std::cout << " v: [" << v.x << "," << v.y << "," << v.z << "]" << std::endl;
    std::cout << " w: [" << w.x << "," << w.y << "," << w.z << "]" << std::endl;
    std::cout << " u: [" << u.x << "," << u.y << "," << u.z << "]" << std::endl;
    std::cout << "gaze: [" << gaze.x << "," << gaze.y << "," << gaze.z << "]" << std::endl;
    std::cout <<"mid point : " << midPoint.x << "  "<< midPoint.y<< "  " << midPoint.z << std::endl;
}

/* Takes coordinate of an image pixel as row and col, and
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
     Ray primaryRay;

     primaryRay.origin.x = position.x;
     primaryRay.origin.y = position.y;
     primaryRay.origin.z = position.z;

     Vector3f pointOnImgPlane;

     float xCoordinate = (row + 0.5) * ((imgPlane.right - imgPlane.left) / imgPlane.nx);
     float yCoordinate = (col + 0.5) * ((imgPlane.top - imgPlane.bottom) / imgPlane.ny); 
     float zCoordinate = (-1)*imgPlane.distance;


     Vector3f su = scalarMultiplication(xCoordinate, u);
     Vector3f sv = scalarMultiplication(yCoordinate, v);
     Vector3f s = vectorAddition(q, vectorSubtraction(su, sv)); 

     Vector3f direction = normalize(vectorSubtraction(s, position));


     primaryRay.direction.x = direction.x;
     primaryRay.direction.y = direction.y;
     primaryRay.direction.z = direction.z;


     return primaryRay;
}
Vector3f Camera::normalize( Vector3f v) const{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (len != 0.0)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}

Vector3f Camera::crossProduct(Vector3f a, Vector3f b)const{
     Vector3f result;

     result.x = a.y * b.z - a.z * b.y;
     result.y = a.z * b.x - a.x * b.z;
     result.z = a.x * b.y - a.y * b.x;

     return result;
}

Vector3f Camera::scalarMultiplication(float t, Vector3f direction) const{
    Vector3f result;

    result.x = direction.x * t;
    result.y = direction.y * t;
    result.z = direction.z * t;

    return result;
}

Vector3f Camera::vectorSubtraction(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;

}
Vector3f Camera::vectorAddition(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;

}

float Camera::dotProduct(Vector3f a, Vector3f b) const{
    
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

