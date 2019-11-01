#include "Shape.h"
#include "Scene.h"
#include <cstdio>

// not default
#include <math.h>
#include <limits>

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
    



    // cout <<"sphere center: [ " 
    // << center.x 
    // << " , "<< center.y
    // << " , "<< center.z
    // << " ] " << endl;


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
    result.id = id;
    result.matIndex = matIndex;
    float t1 = std::numeric_limits<double>::infinity(), t2=std::numeric_limits<double>::infinity();
    bool isDeltaPositive = false;
    Vector3f intersectionPoint1, intersectionPoint2;

    float a = dotProduct(ray.direction, ray.direction);
    // float a = 1;
    float b = 2 *
              dotProduct(
                  ray.direction, 
                  vectorSubtraction(ray.origin, center)
              );
    float c = dotProduct(
                vectorSubtraction(ray.origin, center), 
                vectorSubtraction(ray.origin, center)
              ) - R*R;

    float delta = (b * b) - (4 * a * c);

    if(delta > (-1)*pScene->intTestEps){
        t1 = ((-1) * b + sqrt(delta)) / (2 * a);
        t2 = ((-1) * b - sqrt(delta)) / (2 * a);

        float tmp;
        if(t1 > t2 && t2 >= 0 + pScene->intTestEps){
            tmp = t1;
            t1 = t2;
            t2 = tmp;
        }
        result.t1 = t1;
        result.t2 = t2;
        isDeltaPositive = true;
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

        Vector3f normalVectorOfPoint = normal(intersectionPoint1);

        result.normal[0] = normalVectorOfPoint.x;
        result.normal[1] = normalVectorOfPoint.y;
        result.normal[2] = normalVectorOfPoint.z;
    }

    result.isIntersects = isDeltaPositive;
    result.type = 's';
    // if (result.isIntersects)
    // {
    //     cout << "hits sphere" << endl;
    // }
    // else
    // {
    //     cout << "do not     hits sphere" << endl;
    // }
    

    
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
Vector3f Sphere::normal(Vector3f intersectionPoint)const{
    Vector3f result = scalarMultiplication((1/R), vectorSubtraction(intersectionPoint, center));
    return result;
}
Vector3f Sphere::normalize( Vector3f v) const{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (len != 0.0)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}


////////////////////////////////////////////////////////////////////////////sphere ends/////////////////////
///////////////////////////////////////////////////////////triangle starts///////////////////////////

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
    this->id = id;
    this->matIndex = matIndex;
    this->p1Index = p1Index;
    this->p2Index = p2Index;
    this->p3Index = p3Index;

    p1.x = pVertices->at(p1Index-1).x;
    p1.y = pVertices->at(p1Index-1).y;
    p1.z = pVertices->at(p1Index-1).z;

    p2.x = pVertices->at(p2Index-1).x;
    p2.y = pVertices->at(p2Index-1).y;
    p2.z = pVertices->at(p2Index-1).z;

    p3.x = pVertices->at(p3Index-1).x;
    p3.y = pVertices->at(p3Index-1).y;
    p3.z = pVertices->at(p3Index-1).z;

    // cout <<"triangle: [ " 
    // << p1.x 
    // << " , "<< p1.y
    // << " , "<< p1.z
    // << " ] - [ " << p2.x 
    // << " , "<< p2.y
    // << " , "<< p2.z
    // << " ] - [ "<< p3.x 
    // << " , "<< p3.y
    // << " , "<< p3.z
    // << " ] " <<endl;
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
    result.id = id;
    float t1 = std::numeric_limits<double>::infinity(), t2=std::numeric_limits<double>::infinity();
    result.t1 = std::numeric_limits<double>::infinity(), result.t2=std::numeric_limits<double>::infinity();

    Vector3f origin = normalize(ray.origin);
    Vector3f direction = normalize(ray.direction);

    Vector3f B = vectorSubtraction(p1, origin);
    float beta, gamma, t;

    Vector3f A1 = vectorSubtraction(p1, p2);
    Vector3f A2 = vectorSubtraction(p1, p3);

    
    float determinantA       = determinant(A1, A2, direction);
    float determinantBeta    = determinant( B, A2, direction);
    float determinantGamma   = determinant(A1,  B, direction);
    float determinantT       = determinant(A1, A2, B);

    if((determinantA > (-1)*pScene->intTestEps || 
        determinantA < pScene->intTestEps))
    { //TODO:clerify
        
        // Gramer's Rule for finding determinant of gamma, beta, time
        beta = determinantBeta/determinantA;

        gamma = determinantGamma/determinantA;

        t = determinantT/determinantA;
        // cout << "t:           "<< t << endl;
        if(t < pScene->intTestEps){
            result.isIntersects = false;
        }
        else if
        (
            (gamma < (0+ (-1)*pScene->intTestEps)) || 
            (gamma > (1+(-1)*pScene->intTestEps))
        )
        {
            result.isIntersects = false;
        }
        else if
        (
            (beta < (0+(-1)*pScene->intTestEps)) ||
           // (beta > ((1 - gamma)+(-1)*pScene->intTestEps))
           (beta > ((1 - gamma)+pScene->intTestEps))
        )
        {
            result.isIntersects = false;
        }
        else
        {
            result.isIntersects = true;
            result.t1 = t;
            result.type = 't';
            Vector3f normalVector = normalize(normal(vectorSubtraction(p3,p2),vectorSubtraction(p1,p2)));
            result.normal[0] = normalVector.x;
            result.normal[1] = normalVector.y;
            result.normal[2] = normalVector.z;
        }
    }else{
        result.isIntersects = false;
    }

    // cout <<"triangle: [ " 
    // << p1.x 
    // << " , "<< p1.y
    // << " , "<< p1.z
    // << " ] - [ " << p2.x 
    // << " , "<< p2.y
    // << " , "<< p2.z
    // << " ] - [ "<< p3.x 
    // << " , "<< p3.y
    // << " , "<< p3.z
    // << " ] " <<endl;

    return result;
}
Vector3f Triangle::normal( Vector3f a, Vector3f b) const{
    Vector3f result;

    result.x = a.y * b.z - a.z*b.y;
    result.y = a.z * b.x - a.x*b.z;
    result.z = a.x * b.y - a.y*b.x;

    return result;
}
Vector3f Triangle::normalize( Vector3f v) const{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (len != 0.0)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}
float Triangle::determinant(Vector3f a, Vector3f b, Vector3f c) const{
    return a.x*b.y*c.z + b.x*c.y*a.z + c.x*a.y*b.z - a.z*b.y*c.x - b.z*c.y*a.x - c.z*a.y*b.x;
}
Vector3f Triangle::scalarMultiplication(float t, Vector3f direction) const{
    Vector3f result;

    result.x = direction.x * t;
    result.y = direction.y * t;
    result.z = direction.z * t;

    return result;
}

Vector3f Triangle::vectorSubtraction(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}
Vector3f Triangle::vectorAddition(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;

}

float Triangle::dotProduct(Vector3f a, Vector3f b) const{
    
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
///////////////////////////////////////////////////////////triangle ends///////////////////////////

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
    this->id = id;
    this->matIndex = matIndex;
    this->faces = faces;
    this->pIndecies = pIndecies;
    this->vertices = vertices;
    // cout<< "mesh construction" << endl;
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
    ReturnVal result, tmp;
    result.id = id;
    result.matIndex = matIndex;
    float minT = std::numeric_limits<float>::infinity();
    for (auto i = faces.begin(); i != faces.end(); i++){
        tmp = (*i).intersect(ray);
        if(tmp.t1 < 0.000 - pScene->intTestEps){ // TODO: check time 
            result.isIntersects = false;
        }
        else if (tmp.isIntersects && minT > tmp.t1)
        {
            minT = tmp.t1;

            result.normal[0] = tmp.normal[0];
            result.normal[1] = tmp.normal[1];
            result.normal[2] = tmp.normal[2];

            result.intersectionPoint1[0] = tmp.intersectionPoint1[0];
            result.intersectionPoint1[1] = tmp.intersectionPoint1[1];
            result.intersectionPoint1[2] = tmp.intersectionPoint1[2];

            result.intersectionPoint2[0] = tmp.intersectionPoint2[0];
            result.intersectionPoint2[1] = tmp.intersectionPoint2[1];
            result.intersectionPoint2[2] = tmp.intersectionPoint2[2];

            result.isIntersects = tmp.isIntersects;

            result.t1 = tmp.t1;
            result.t2 = tmp.t2;

            result.type = tmp.type;
        }   
    }
    

    return result;
}