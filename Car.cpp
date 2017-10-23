#include "Car.h"
#include "math.h"

void Car::init(int id,float pos[3], float rotAxis[3], float rot)
{
	Object::init(id,pos,rotAxis,rot);
	float wheelRotAxis[3] = { 0.0f,0.0f,0.1f };
	int wheelId = objId + 1;
	for (int i = 0; i < 4; i++,wheelId ++) {
		float wpos[3] = { xWheelpos[i], -0.5f, yWheelpos[i] };
		wheels[i] = new Wheel();
		wheels[i]->init(wheelId,wpos,wheelRotAxis,90);
	}
	for (int i = 0; i < 2; i++) {
		float pos[4] = {0.f+i,.5f,0,1};//{0+i,0.5,0,1};
		float dir[4] = { 0.f,1,0,0 };
		headlights[i] = new LightSource();
		headlights[i]->setSpot(pos,dir, 120);
	}
	//store initPos to use when car respawns
	setInitialPos(pos);
}

void Car::init(int id, float pos[3], float rotAxis[3], float rot, float size[3])
{
	Car::init(id,  pos,  rotAxis,  rot);
	for (int i = 0; i < 3; i++)
		size3[i] = size[i];

	//store initPos to use when car respawns
	setInitialPos(pos);
}

void Car::move(int  dir) {
	float dx, dy;
	switch (dir) {
	case back:
		dy = (float)(cos(rotation * PI / 180.0) * speed);
		dx = (float)(sin(rotation* PI / 180.0) * speed);
		position[0] -= dx;
		position[2] -= dy;
		break;
	case forward:
		dy = (float)(cos(rotation * PI / 180.0) * speed);
		dx = (float)(sin(rotation* PI / 180.0) * speed);
		position[0] += dx;
		position[2] += dy;
		break;
	}
	for (int i = 0; i < 4; i++) {
		wheels[i]->rotate(dir);
	}
	for (int i = 0; i < 2; i++) {
		headlights[i]->move(dir,dx,dy);
	}
}

void Car::rotate(int dir) {
	switch (dir) {
	case left:
		rotation += 1;
		if (rotation > 360) {
			rotation = 0;
		}
		break;
	case right:
		rotation -= 1;
		if (rotation < 0) {
			rotation = 360;
		}
		break;
	}
	for (int i = 0; i < 2; i++) {
		headlights[i]->rotate(rotation);
	}
}

void Car::collided()
{
	if(rotation <= 180){
	if (collisionPenetration3[0] > 0)
		move(forward);
	else
		move(back);
	}
	else
		move(forward);
	/*position[0] -= collisionPenetration3[0];
	position[2] -= collisionPenetration3[2];*/
	collisionPenetration3[0] = 0;
	collisionPenetration3[2] = 0;
}

//store initPos to use when car respawns
void Car::setInitialPos(float posToSet[3])
{
	for (int i = 0; i < 3; i++)
		initialPos[i] = posToSet[i];
}

//used when we need to move the car (with headlights and wheels) to a specific location, for instance, when the player dies
void Car::moveToPos(float posToMoveTo[3])
{
	//move the car to the new position
	position[0] = posToMoveTo[0];
	position[2] = posToMoveTo[2];
	
	for (int i = 0; i < 2; i++) {
		headlights[i]->l_position[0] = headlights[i]->initialPos[0];
		headlights[i]->l_position[2] = headlights[i]->initialPos[2];
	}
}

void Car::respawn()
{
	moveToPos(initialPos);
}
