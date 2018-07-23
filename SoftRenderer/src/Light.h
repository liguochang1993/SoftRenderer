#pragma once

#include "Vertex.h"
#include "Transform.h"

class Light
{
public:
	Light(Vector d, Color c);
	~Light();
	Vector pos;
	Vector direction;
	Color color;
	Transform *transform;
	float intensity = 0.3f;
	float specularStrength = 0.5f;
};