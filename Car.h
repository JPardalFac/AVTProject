#pragma once
#include "Object.h"
#include "LightSource.h"
#include "Wheel.h"
#include "AVTmathLib.h"

class Car : public Object {

private:
	float xWheelpos[4] = { 0.5f ,0.5f ,-0.5f,-0.5f };
	float yWheelpos[4] = { 0.5f ,-0.5f ,0.5f,-0.5f };
	float speed = 0.1f;
	float initialPos[3];


public:
	float amb[4] = { 0.3f, 0.0f, 0.0f, 1.0f };
	float diff[4] = { 0.5f, 0.0f, 0.0f, 1.0f };
	float spec[4] = { 0.7f, 0.6f, 0.6f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 32.0f;
	int texcount = 0;
	Wheel* wheels[4];
	LightSource* headlights[2];
	float size3[3] = { 1, 1, 1 }; //used for scaling purposes and collision detection

	void init(int id, float pos[3], float rotAxis[3], float rot) override;
	void init(int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	//void setColor(MyMesh mesh[]);
	void move(int dir) override;
	void rotate(int dir)override;
	void collided();
	void setInitialPos(float posToSet[3]);
	void moveToPos(float posToMoveTo[3]);
	void respawn();
};
