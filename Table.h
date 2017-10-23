#pragma once
#include "Object.h"

class Table : public Object {

public:
	float amb[4] = { 0.2f, 0.15f, 0.1f, 1.0f };
	float diff[4] = { 0.8f, 0.6f, 0.4f, 1.0f };
	float spec[4] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float emissive[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float shininess = 100;
	int texcount = 0;

	float x = 50;
	float y = 50;

	Table();
};