#include "Camera.h"



// not default
#include <string.h>

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

     std::cout << "image plane left: " << imgPlane.left<< "   imagePlane right: " << imgPlane.right << std::endl; 

     

     this->gaze.x = gaze.x;
     this->gaze.y = gaze.y;
     this->gaze.z = gaze.z;

     position.x = pos.x;
     position.y = pos.y;
     position.z = pos.z;

     v.x = up.x;
     v.y = up.y;
     v.z = up.z;

    //  w.x = gaze.x;
    //  w.y = gaze.y;
    //  w.z = gaze.z;

     w.x = (-1)*gaze.x;
     w.y = (-1)*gaze.y;
     w.z = (-1)*gaze.z;

     u = crossProduct(v,w);

     std::cout << "camera position: " << std::endl
     << "    u: [" << u.x << ", "<<u.y<<", "<<u.z<< "]" << std::endl 
     << "    v: [" << v.x << ", "<<v.y<<", "<<v.z<< "]" << std::endl
     << "    w: [" << w.x << ", "<<w.y<<", "<<w.z<< "]" << std::endl ;


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

     //TODO: is point on image normalized??
     std::cout << std::endl << "column:  " << col << "       row:  " << row << std:: endl;
     Ray primaryRay;

     primaryRay.origin.x = position.x;
     primaryRay.origin.y = position.y;
     primaryRay.origin.z = position.z;

     Vector3f pointOnImgPlane;

     float xCoordinate = (row + 0.5) * (imgPlane.right - imgPlane.left) / imgPlane.nx;
     float yCoordinate = (col + 0.5) * (imgPlane.top - imgPlane.bottom) / imgPlane.ny;

    //  std::cout<< " x:" << xCoordinate<< " y:"<< yCoordinate<< std::endl;

     pointOnImgPlane.x = xCoordinate;
     pointOnImgPlane.y = yCoordinate;
     pointOnImgPlane.z = gaze.z;

     Vector3f direction = vectorSubtraction(pointOnImgPlane, position);


     primaryRay.direction.x = direction.x;
     primaryRay.direction.y = direction.y;
     primaryRay.direction.z = direction.z;

     //std::cout << "primary ray is created and returned \n";

     return primaryRay;
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

