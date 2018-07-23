#pragma once
#include "Math.h"

class Vertex {

public:
	Vertex(Vector p, Color c);
	~Vertex();
	
	Vector pos;
	Vector world_pos;
	Texcoord tc;
	Color color;
	Vector normal;

	void rhw_init();
	void lerp(Vertex *v0, Vertex *v1, float t);
};