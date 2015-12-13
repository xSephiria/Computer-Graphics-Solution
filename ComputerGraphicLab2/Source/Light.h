#ifndef LIGHT_H
#define LIGHT_H

#include "Vertex.h"

struct Light
{
	Position position;
	Color color;
	float power;
	float kC, kL, kQ;
	Light()
	{
		position = (0.f, 0.f, 0.f);
		color = (0.f, 0.f, 0.f);
		power = 0.f;
		kC = kL = kQ = 0.f;
	}

	enum LIGHT_TYPE
	{
		LIGHT_POINT = 0,
		LIGHT_DIRECTIONAL,
		LIGHT_SPOT,
	};

	LIGHT_TYPE type;
	Vector3 spotDirection;
	float cosCutoff;
	float cosInner;
	float exponent;

};

#endif