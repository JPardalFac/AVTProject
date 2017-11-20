#pragma once

#include "Object.h"

class ButterPacket : public Object
{
	float size3[3] = {1.0, 0.5, 0.5}; //size is used for scaling and collision detection, CUBE COLLIDER
public:
	//materials
	float amb[4] = { 0.05f, 0.05f, 0.0f, 1.0f };
	float diff[4] = { 0.5f, 0.5f, 0.4f, 1.0f };
	float spec[4] = { 0.7f, 0.7f, 0.04f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 10;
	int texcount = 0;

	ButterPacket();
	ButterPacket(int id, float pos[3], float rotAxis[3], float rot);
	void collided();
};