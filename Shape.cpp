#include "Shape.h"
#include "Scene.h"
#include <cstdio>

// not default
#include <math.h> 

//not dependent
#include <iostream>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}

////////////////////////////////////////////////////////////////////////////sphere starts/////////////////////

Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    center.x = pVertices->at(cIndex-1).x;
    center.y = pVertices->at(cIndex-1).y;
    center.z = pVertices->at(cIndex-1).z;

    this->id = id;
    this->matIndex = matIndex;
    this->R = R;
    



    cout <<"sphere center: [ " 
        << center.x 
        << " , "<< center.y
        << " , "<< center.z
        << " ] " << endl;


}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    ReturnVal result;
    cout << "--------------------------------------------------------------------------- " << endl << endl;
    float t1, t2;
    bool isDeltaPositive = true;
    Vector3f intersectionPoint1, intersectionPoint2;

    cout << "ray origin: [" << ray.origin.x << "," << ray.origin.y << "," << ray.origin.z << "]" << endl;
    cout << "ray direction: [" <<  ray.direction.x << "," << ray.direction.y << "," << ray.direction.z << "]" << endl;
    cout << "sphere center: [" <<  center.x << "," << center.y << "," << center.z << "]" << endl;
    cout << "sphere radius: " << R << endl;


    // float a = dotProduct(ray.direction, ray.direction);
    float a = 1;
    float b = 2 *
        dotProduct(
            ray.direction, 
            vectorSubtraction(
                ray.origin, center
            )
        );
    float c = dotProduct(
                vectorSubtraction(ray.origin, center),
                vectorSubtraction(ray.origin, center)
              ) - R*R;

    float delta = (b * b) - (4 * a * c);

    cout << "a:" << a << "     b:" << b << "     c:" << c << "     delta: " << delta << endl;

    if(delta < 0){
        isDeltaPositive = false;
        // cout << "ray do not intersects, delta = "<< delta << endl;
    }else{
        t1 = ((-1) * b + sqrt(delta)) / (2 * a);
        t2 = ((-1) * b - sqrt(delta)) / (2 * a);

        float tmp;
        if(t1 > t2){
            tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        result.t1 = t1;
        result.t2 = t2;
        cout << ">>>>>>>>>>>>>>>>>>>>t1:" << (((-1) * b + sqrt(b * b - delta)) / (2 * a)) << " t2:" << t2 << " delta:" << delta << endl;
    }

    if(isDeltaPositive){
        intersectionPoint1 = ray.getPoint(t1);
        intersectionPoint2 = ray.getPoint(t2);

        result.intersectionPoint1[0] = intersectionPoint1.x;
        result.intersectionPoint1[1] = intersectionPoint1.y;
        result.intersectionPoint1[2] = intersectionPoint1.z;
        
        result.intersectionPoint2[0] = intersectionPoint2.x;
        result.intersectionPoint2[1] = intersectionPoint2.y;
        result.intersectionPoint2[2] = intersectionPoint2.z;
    }

    result.isIntersects = isDeltaPositive;
    result.type = 's';

    
    return result;
}

Vector3f Sphere::scalarMultiplication(float t, Vector3f direction) const{
    Vector3f result;

    result.x = direction.x * t;
    result.y = direction.y * t;
    result.z = direction.z * t;

    return result;
}

Vector3f Sphere::vectorSubtraction(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}
Vector3f Sphere::vectorAddition(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;

}

float Sphere::dotProduct(Vector3f a, Vector3f b) const{
    
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}


////////////////////////////////////////////////////////////////////////////sphere ends/////////////////////

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    cout <<"triangle-- " 
        << " [ "<<(*pVertices)[p1Index-1].x 
        << " , "<<(*pVertices)[p2Index-1].y
        << " , "<<(*pVertices)[p3Index-1].z
        << " ]" << endl;
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    ReturnVal result;

    result.t1 = 0;
    result.t2 = 0;

    result.intersectionPoint1[0] = 0;
    result.intersectionPoint1[1] = 0;
    result.intersectionPoint1[2] = 0;

    result.intersectionPoint2[0] = 0;
    result.intersectionPoint2[1] = 0;
    result.intersectionPoint2[2] = 0;
    result.isIntersects = false;

    result.type = 't';
    return result;
}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces, vector<int> *pIndices, vector<Vector3f> *pVertices)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    ReturnVal result;

    result.t1 = 0;
    result.t2 = 0;

    result.intersectionPoint1[0] = 0;
    result.intersectionPoint1[1] = 0;
    result.intersectionPoint1[2] = 0;

    result.intersectionPoint2[0] = 0;
    result.intersectionPoint2[1] = 0;
    result.intersectionPoint2[2] = 0;
    result.isIntersects = false;
    result.type = 'm';

    return result;
}
