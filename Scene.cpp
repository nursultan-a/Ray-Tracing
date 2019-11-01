#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"
#include "tinyxml2.h"

//not default
#include <typeinfo>
#include "Image.h"
#include <limits>
#include <math.h>


using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */
void Scene::renderScene(void)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	for (auto camera = cameras.begin(); camera != cameras.end(); camera++)
	{
		Image renderedImage((*camera)->imgPlane.nx, (*camera)->imgPlane.ny);
		unsigned long int counter=0;
		unsigned long int total=0;

		for(int y = 0 ; y < (*camera)->imgPlane.ny; y++)
		{
			for(int x = 0 ; x < (*camera)->imgPlane.nx; x++)
			{
				Ray primaryRay = (*camera)->getPrimaryRay(y, x);

				Color values{0,0,0};
				// Color values{backgroundColor.r,backgroundColor.g,backgroundColor.};

				float minT = std::numeric_limits<float>::infinity(); // INFINITY
				float infinity = std::numeric_limits<float>::infinity(); // INFINITY

				ReturnVal hitPoint;

				for (auto object = objects.begin(); object != objects.end(); object++)
				{	
					ReturnVal intersectionDetails = (*object)->intersect(primaryRay);
					if(intersectionDetails.isIntersects && minT > intersectionDetails.t1){

						// cout << "minT: " << minT << endl;

						// hitPoint = intersectionDetails; // shallow copy, what happens to array variables?
						hitPoint.id = intersectionDetails.id;
						hitPoint.matIndex = intersectionDetails.matIndex;
						hitPoint.t1 = intersectionDetails.t1;
						hitPoint.t2 = intersectionDetails.t2;
						hitPoint.type = intersectionDetails.type;
						hitPoint.isIntersects = intersectionDetails.isIntersects;


						hitPoint.intersectionPoint1[0] = intersectionDetails.intersectionPoint1[0];
						hitPoint.intersectionPoint1[1] = intersectionDetails.intersectionPoint1[1];
						hitPoint.intersectionPoint1[2] = intersectionDetails.intersectionPoint1[2];

						hitPoint.intersectionPoint2[0] = intersectionDetails.intersectionPoint2[0];
						hitPoint.intersectionPoint2[1] = intersectionDetails.intersectionPoint2[1];
						hitPoint.intersectionPoint2[2] = intersectionDetails.intersectionPoint2[2];

						hitPoint.normal[0] = intersectionDetails.normal[0];
						hitPoint.normal[1] = intersectionDetails.normal[1];
						hitPoint.normal[2] = intersectionDetails.normal[2];

						minT = intersectionDetails.t1;
					}					
				}
				Vector3f diffuseLight = {50,50,50};
				if(hitPoint.isIntersects && minT < infinity && minT >= (-1)*intTestEps){
					
					for (auto i = lights.begin(); i != lights.end(); i++)
					{
						Vector3f point;
						point.x = hitPoint.intersectionPoint1[0];
						point.y = hitPoint.intersectionPoint1[1];
						point.z = hitPoint.intersectionPoint1[2];

						Vector3f intensityOverDistance = (*i)->computeLightContribution(point);
						Vector3f wi = vectorSubtraction((*i)->position, point);
						Vector3f diffuseRef = materials[hitPoint.matIndex-1]->diffuseRef;
						

						float cosTheta = 0;
						Vector3f normal;

						normal.x = hitPoint.normal[0];
						normal.y = hitPoint.normal[1];
						normal.z = hitPoint.normal[2];

						// normal = normal);
						// cout << normal.x << ", " << normal.y << ", " << normal.z << "type: "<<hitPoint.type <<endl;
						if(dotProduct(normalize(wi), normal)>0.0){
							cosTheta = dotProduct(normalize(wi), normal);
						}

						diffuseRef = scalarMultiplication(cosTheta, diffuseRef);

						Vector3f diffuseShading;
						diffuseShading.r = diffuseRef.r * intensityOverDistance.r;
						diffuseShading.g = diffuseRef.g * intensityOverDistance.g;
						diffuseShading.b = diffuseRef.b * intensityOverDistance.b;

						diffuseLight.r += diffuseShading.r;
						diffuseLight.g += diffuseShading.g;
						diffuseLight.b += diffuseShading.b;
						// cout <<"light position  "<<(*i)->position.x<<", "<<(*i)->position.y<<", "<<(*i)->position.x << endl;
					}
					
						Vector3f ambi;
						ambi.x = materials[hitPoint.matIndex-1]->ambientRef.x * ambientLight.x;
						ambi.y = materials[hitPoint.matIndex-1]->ambientRef.y * ambientLight.y;
						ambi.z = materials[hitPoint.matIndex-1]->ambientRef.z * ambientLight.z;
						values.red = diffuseLight.r + ambi.x;
						values.grn = diffuseLight.g + ambi.y;
						values.blu = diffuseLight.b + ambi.z;

						counter++;
				}
				else{
					values.red = backgroundColor.x;
					values.grn = backgroundColor.y;
					values.blu = backgroundColor.z;
				}
				total++;
				renderedImage.setPixelValue(y,x, values);
			}
		}
		renderedImage.saveImage((*camera)->imageName);
		cout << endl<<"total colored pixels: " << counter << "  total pixel: " <<total <<endl << endl;
	}

	
	
	
}
////////////////// Private functions starts//////////////////////
Vector3f Scene::normal( Vector3f a, Vector3f b) const{
    Vector3f result;

    result.x = a.y * b.z - a.z*b.y;
    result.y = a.z * b.x - a.x*b.z;
    result.z = a.x * b.y - a.y*b.x;

    return result;
}
Vector3f Scene::normalize( Vector3f v) const{
    float len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

    if (len != 0.0)
    {
        v.x /= len;
        v.y /= len;
        v.z /= len;
    }
    return v;
}
float Scene::determinant(Vector3f a, Vector3f b, Vector3f c) const{
    return a.x*b.y*c.z + b.x*c.y*a.z + c.x*a.y*b.z - a.z*b.y*c.x - b.z*c.y*a.x - c.z*a.y*b.x;
}
Vector3f Scene::scalarMultiplication(float t, Vector3f direction) const{
    Vector3f result;

    result.x = direction.x * t;
    result.y = direction.y * t;
    result.z = direction.z * t;

    return result;
}

Vector3f Scene::vectorSubtraction(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;

    return result;
}
Vector3f Scene::vectorAddition(Vector3f a, Vector3f b) const{
    Vector3f result;
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;

    return result;

}

float Scene::dotProduct(Vector3f a, Vector3f b) const{
    
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}
////////////////// Private functions ends //////////////////////

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R, &vertices));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index, &vertices));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;
		vector<int> *meshIndices = new vector<int>;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index, &vertices)));
			meshIndices->push_back(p1Index);
			meshIndices->push_back(p2Index);
			meshIndices->push_back(p3Index);
		}

		objects.push_back(new Mesh(id, matIndex, faces, meshIndices, &vertices));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

