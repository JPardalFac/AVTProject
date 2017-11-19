#include "Orange.h"

Orange::Orange()
{
}

Orange::Orange(int id, float pos[3], float rotAxis[3], float rot)
{
	Object::init(id, pos, rotAxis, rot);
}

void Orange::move(int dir)
{
}

void Orange::update()
{
	rotation++;
	currentRot++;
	for (int i = 0; i < 3; i++) {
		position[i] += speed[i] * direction[i];
		if (position[i] > 25 || position[i] < -25)
			direction[i] *= -1;
	}
}

void Orange::updateSpeed(float values[3])
{
	for(int i = 0; i < 3; i++)
		speed[i] = values[i];
}

void Orange::updateSpeedAt(float value, int index)
{
	speed[index] = value;
}

void Orange::updateSpeed()
{
	for (int i = 0; i < 3; i++)
		speed[i] += 0.01;
}

void Orange::collided()
{
	for (int i = 0; i < 3; i++)
	{
		direction[i] *= -1;
	}
	direction[0] += 0.14;
	direction[2] += 0.14;
}
