#pragma once

#include "Object.h"

class Cheerio : public Object 
{
	//float size3[3] = {0.5, 0.5, 0.5}; //size is used for scaling and collision detection, CUBE COLLIDER
public:
	//materials
	float amb[4] = { 0.2f, 0.15f, 0.7f, 1.0f };
	float diff[4] = { 0.5f, 0.5f, 0.7f, 1.0f };
	float spec[4] = { 0.5f, 0.5f, 0.8f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 64;
	int texcount = 0;

	//Torus creation values
	float initValues[4] = { 0.2f, 0.5f, 15, 5 };

	Cheerio();
	//Cheerio(std::string fileName, int id, float pos[3], float rotAxis[3], float rot);
	//Cheerio(std::string fileName, int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	Cheerio(int id, float pos[3], float rotAxis[3], float rot);
	Cheerio( int id, float pos[3], float rotAxis[3], float rot, float size[3]);
	void collided();
};
