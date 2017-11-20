#include "Object.h"

void Object::init(int id, float pos[3], float rotAxis[3], float rot) {

	objId = id;

	//set obj position
	position[0] = pos[0];
	position[1] = pos[1];
	position[2] = pos[2];

	//set rotation axis
	rotationAxis[0] = rotAxis[0];
	rotationAxis[1] = rotAxis[1];
	rotationAxis[2] = rotAxis[2];

	//set rotation value
	rotation = rot;

	//set velocity
	for (int i = 0; i < 3; i++)
		velocity[i] = 0;
}

void Object::init(int id, float pos[3], float rotAxis[3], float rot, float size[3])
{
	init(id, pos, rotationAxis, rot);
	for (int i = 0; i < 3; i++)
		size3[i] = size[i];

	//set velocity
	for (int i = 0; i < 3; i++)
		velocity[i] = 0;
}

void Object::move(int dir)
{
}

void Object::rotate(int dir)
{
}

//using AABB algorithm
bool Object::checkCollision(Object & one, Object & two)
{
	float topLeftOne[3], topLeftTwo[3];
	//calculate the topLeft position for the first obj
	topLeftOne[0] = one.position[0] - one.size3[0] / 2;
	topLeftOne[2] = one.position[2] - one.size3[2] / 2;
	topLeftOne[1] = 1;

	//calculate the topLeft position for the second obj
	topLeftTwo[0] = two.position[0] - two.size3[0] / 2;
	topLeftTwo[2] = two.position[2] - two.size3[2] / 2;
	topLeftTwo[1] = 1;

	bool collisionX = topLeftOne[0] + one.size3[0] >= topLeftTwo[0] &&
					  topLeftTwo[0] + two.size3[0] >= topLeftOne[0];
	if (!collisionX)
		return false; //if there is no collision in x, there is no need to check for Y

	bool collisionY = topLeftOne[2] + one.size3[2] >= topLeftTwo[2] &&
					  topLeftTwo[2] + two.size3[2] >= topLeftOne[2];

	if (collisionX && collisionY)
		calculateCollisionPenetration(topLeftOne, topLeftTwo, one.size3, two.size3);
	return collisionX && collisionY;
}

//if two objects are colliding, this function will calculate how much they are inside one another, and store this values on the first Object
void Object::calculateCollisionPenetration(float obj1[3], float sizeObj1[3], float obj2[3], float sizeObj2[3])
{
	//calculate the x penetration, this value is going to be the offset the car needs to move back in order to cancel the collision
	if (obj1[0] < obj2[0])
		collisionPenetration3[0] = obj1[0] + sizeObj1[0] - obj2[0];
	else
		collisionPenetration3[0] = obj2[0] + sizeObj2[0] - obj1[0];

	//calculate the z penetration, this value is going to be the offset the car needs to move back in order to cancel the collision
	if(obj1[2] < obj2[2])
		collisionPenetration3[2] = obj1[2] + sizeObj1[2] - obj2[2];
	else
		collisionPenetration3[2] = obj2[2] + sizeObj2[2] - obj1[2];
}

void Object::collided()
{
}

void Object::init(std::string fileName, int id, float pos[3], float rotAxis[3], float rot) {//

	objId = id;

	//set obj position
	position[0] = pos[0];
	position[1] = pos[1];
	position[2] = pos[2];

	//set rotation axis
	rotationAxis[0] = rotAxis[0];
	rotationAxis[1] = rotAxis[1];
	rotationAxis[2] = rotAxis[2];

	//set rotation value
	rotation = rot;

	if (fileName != "") {
		obj = new ObjectFromLoad();
		if (!obj->Import3DFromFile(fileName))
			std::cout << "FAIL LOAD OBJ" << std::endl;
		else {
			//objects[i]->LoadGLTextures();
			obj->genVAOsAndUniformBuffer();
		}
		for (int i = 0; i < obj->object->mNumMeshes; i++) {
			meshesNames.push_back( obj->myMeshes[i].name);
		}
	}
}

void Object::init( std::string fileName,int id, float pos[3], float rotAxis[3], float rot, float size[3])//
{
	init(fileName,id, pos, rotationAxis, rot);
	for (int i = 0; i < 3; i++)
		size3[i] = size[i];
}

void Object::setMaterial(int n, float amb[4], float diff[4], float spec[4], float emiss[4], float shi, int texC) {
	obj->setMaterial(n, amb,diff,spec,emiss,shi,texC);
}

void Object::drawObject(GLuint programId) {
	obj->renderModel(programId);
}



