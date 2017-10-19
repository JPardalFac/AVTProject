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
}
//
//void Car::setColor(MyMesh mesh[10]) {
//	
//	Object::setColor(mesh,amb,diff,spec,emissive,shininess,texcount);
//	createCube();
//	//for (int i = 0; i < 4; i++) {
//	//	wheels[i]->setColor(mesh);
//	//}
//}



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