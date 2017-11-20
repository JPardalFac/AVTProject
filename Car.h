#pragma once
#include "Object.h"
#include "LightSource.h"
#include "Wheel.h"
#include "AVTmathLib.h"
#include "Table.h"

class Car : public Object {

private:
	float initialX		 = -50 / 2 + 5; //-50 -> tableX

	float xWheelpos[4] = { 0.5f ,0.5f ,-0.5f,-0.5f };
	float yWheelpos[4] = { 0.5f ,-0.5f ,0.5f,-0.5f };
	float speed = 0.1;
	std::string meshName;
	
	bool canMoveForward = true;
	bool canMoveBackward = true;
	bool isColliding = false;

public:
	//initialization values
	float initialRot = 135;
	float RotAxis[3] = { 0, 1, 0 };
	float initialPos[3] = { initialX, 0.7f, -.5f };

	float amb[4] = { 0.2f, 0.15f, 0.7f, 1.0f };
	float diff[4] = { 0.3f, 0.3f, 0.7f, 1.0f };
	float spec[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 16;
	int texcount = 0;
	Wheel* wheels[4];
	LightSource* headlights[2];
	float size3[3] = { 1, 1, 1 }; //used for scaling purposes and collision detection

	void init(std::string fileName, int id, float pos[3], float rotAxis[3], float rot) override;
	void init(std::string fileName, int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	void init(int id, float pos[3], float rotAxis[3], float rot) override;
	void init(int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	void move(int dir) override;
	void rotate(int dir)override;
	void collided(Object collidedWith);
	void setInitialPos(float posToSet[3]);
	void moveToPos(float posToMoveTo[3]);
	void respawn();
	void resetRotation();
	void resetCollisionFlag();
};
