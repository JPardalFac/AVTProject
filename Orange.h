#pragma once
#include "Object.h"

class Orange : public Object{

private:
	
public:
	float amb[4] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[4] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100;
	int texcount = 0;
	float currentRot = 1.0f;
	float movementRotAxis[3] = { 0.0f,0.0f,1.0f };
	float direction[3] = { 0.0f, 0.0f, 1.0f };
	float speed[3] = { 0.1f, 0.0f, 0.1f };
	float radius = 0.5f;
	int divisions = 20;

	Orange();
	Orange(int id, float pos[3], float rotAxis[3], float rot);
	//virtual void setColor(MyMesh mesh[],float amb[4],float diff[4],float spec[4],float emissive[4],float shininess,int texcount);
	//virtual void createObj();
	void move(int dir);
	void update();
	void updateSpeed(float values[3]);
	void updateSpeedAt(float value, int index);
	void updateSpeed();
	void collided() override;
};
