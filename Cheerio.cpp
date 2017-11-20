#include "Cheerio.h"

Cheerio::Cheerio() 
{
}

Cheerio::Cheerio( int id, float pos[3], float rotAxis[3], float rot)//std::string fileName,
{
	Object::init(id, pos, rotAxis, rot);//fileName,
	for (int i = 0; i < 3; i++)
		size3[i] = initValues[1] * 2;	//size of the collider should be the outerRadius * 2
}

Cheerio::Cheerio( int id, float pos[3], float rotAxis[3], float rot, float size[3])//std::string fileName,
{
	Object::init(id, pos, rotAxis, rot, size);//fileName,
}

void Cheerio::collided(float carPos[3])
{
	float deltaX = carPos[0] - position[0];	//calculate the vector between the center of both objs
	float deltaZ = carPos[2] - position[2];
	
	velocity[0] = deltaX; //use the previously calculated values to create the velocity vector for the cheerio
	velocity[2] = deltaZ;

	movementUpdate();
}

void Cheerio::movementUpdate()
{
	position[0] -= velocity[0] * velocityModifier;
	position[2] -= velocity[2] * velocityModifier;
	
}

