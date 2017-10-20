#pragma once
#include "Object.h"
#include "LightSource.h"
#include "Wheel.h"

class Car : public Object {

private:
	float xWheelpos[4] = { 0.5f ,0.5f ,-0.5f,-0.5f };
	float yWheelpos[4] = { 0.5f ,-0.5f ,0.5f,-0.5f };
	float speed = 0.1f;


public:
	float amb[4] = { 0.3f, 0.0f, 0.0f, 1.0f };
	float diff[4] = { 0.5f, 0.0f, 0.0f, 1.0f };
	float spec[4] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 32.0f;
	int texcount = 0;
	Wheel* wheels[4];
	LightSource* headlights[2];


	void init(int id, float pos[3], float rotAxis[3], float rot) override;
	//void setColor(MyMesh mesh[]);
	void move(int dir) override;
	void rotate(int dir)override;
	//void createObj()override;
};
