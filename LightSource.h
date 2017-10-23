#pragma once
#include <string>

#define PI 3.1415289
class LightSource {
private:
	std::string type;
	enum direction { back, forward, left, right };

public:
	float l_position[4];
	float direction[4];
	float spot_cutoff;
	float speed = 0.15f;

	void setSpot(float pos[4], float dir[4],float cutoff);
	void setPoint(float pos[4]);
	void setAmb(float dir[4]);

	void rotate(float rot, float car_pos[4], float des);
	void move(int direction, float dx, float dy);
};