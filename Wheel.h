#pragma once
#include "Object.h"

class Wheel : public Object {

private:
	float speed = 4.f;


public:
	float amb[4] = { 0.3f, 0.0f, 0.3f, 1.0f };
	float diff[4] = { 0.5f, 0.0f, 0.4f, 1.0f };
	float spec[4] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 32.0f;
	int texcount = 0;
	
	float initValues[4] = { 0.1f, 0.2f, 15, 5 };
	float currentRot = 0;
	float movementRotAxis[3] = { 1.0f,0.0f,0.0f };

	void rotate(int dir)override;
	//void createObj() override;
	//void setColor(MyMesh mesh[10]);

};