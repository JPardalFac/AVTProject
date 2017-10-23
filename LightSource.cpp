#include "LightSource.h"

void LightSource::setSpot(float pos[4], float dir[4], float cutoff)
{
	l_position[0] = pos[0];
	l_position[1] = pos[1];
	l_position[2] = pos[2];
	l_position[3] = pos[3];

	direction[0] = dir[0];
	direction[1] = dir[1];
	direction[2] = dir[2];
	direction[3] = dir[3];

	for (int i = 0; i < 4; i++)
		initialPos[i] = pos[i];

	spot_cutoff = cutoff;

	type = "spot";
}

void LightSource::setPoint(float pos[4])
{
	l_position[0] = pos[0];
	l_position[1] = pos[1];
	l_position[2] = pos[2];
	l_position[3] = pos[3];

	type = "point";
}

void LightSource::setAmb(float dir[4])
{
	direction[0] = dir[0];
	direction[1] = dir[1];
	direction[2] = dir[2];
	direction[3] = dir[3];

	type = "amb";
}

void LightSource::rotate(float rot)
{
	direction[0] = cos((rot* PI / 180.0)+(PI / 2.f));// *speed;
	direction[1] = sin((rot* PI / 180.0)+(PI / 2.f));// *speed;
}

void LightSource::move(int direction, float dx, float dy)
{
	
	switch (direction) {
	case forward:
		l_position[0] += dx;
		l_position[2] += dy;
		break;
	case back:
		l_position[0] -= dx;
		l_position[2] -= dy;
		//currentRot -= 4;
		break;
	}
}
