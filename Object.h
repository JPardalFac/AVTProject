#pragma once
#include <sstream>
#include <iostream>
//#include "basic_geometry.h"
#include "ObjLoader.h" 

class Object {
protected:
	enum direction { back, forward, left, right };
	
public:
	int objId;
	float position[3];
	float rotationAxis[3];
	float rotation;
	ObjectFromLoad* obj;
	//std::string meshesNames[30];
	std::vector<std::string> meshesNames;

	float velocity[3];
	
	//collision variables
	float size3[3] = {1, 1, 1}; //used for collision detection. This assumes CUBE COLLIDER
	float collisionPenetration3[3] = { 0, 0, 0 };

	virtual void init(int id, float pos[3], float rotAxis[3], float rot);
	virtual void init(int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	virtual void init(std::string fileName, int id, float pos[3], float rotAxis[3], float rot);
	virtual void init(std::string fileName,int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	//virtual void setColor(MyMesh mesh[],float amb[4],float diff[4],float spec[4],float emissive[4],float shininess,int texcount);
	//virtual void createObj();
	virtual void move(int dir);
	virtual void rotate(int dir);
	bool checkCollision(Object &one, Object &two);
	virtual void collided();
	void calculateCollisionPenetration(float obj1[3], float obj1Size[3], float obj2[3], float obj2Size[3]);
	void drawObject(GLuint programId);
	virtual void setMaterial(int n, float amb[4], float diff[4], float spec[4], float emiss[4], float shi, int texC);
	
};


