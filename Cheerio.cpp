#include "Cheerio.h"

Cheerio::Cheerio() 
{
}

Cheerio::Cheerio(int id, float pos[3], float rotAxis[3], float rot)
{
	Object::init(id, pos, rotAxis, rot);
	for (int i = 0; i < 3; i++)
		size3[i] = initValues[1] * 2;	//size of the collider should be the outerRadius * 2
}

Cheerio::Cheerio(int id, float pos[3], float rotAxis[3], float rot, float size[3])
{
	Object::init(id, pos, rotAxis, rot, size);
}

void Cheerio::collided()
{
}
