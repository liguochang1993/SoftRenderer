#include "Vertex.h"

Vertex::Vertex(Vector p, Color c) {
	pos = p;
	color = c;
}

Vertex::~Vertex() {}

void Vertex::rhw_init() {
	color.r /= pos.z;
	color.g /= pos.z;
	color.b /= pos.z;
	tc.u /= pos.z;
	tc.v /= pos.z;
	normal.x /= pos.z;
	normal.y /= pos.z;
	normal.z /= pos.z;
	world_pos.x /= pos.z;
	world_pos.y /= pos.z;
	world_pos.z /= pos.z;
}

void Vertex::lerp(Vertex *v0, Vertex *v1, float t) {
	pos.z = Math::lerp(v0->pos.z, v1->pos.z, t);
	color.r = Math::lerp(v0->color.r, v1->color.r, t);
	color.g = Math::lerp(v0->color.g, v1->color.g, t);
	color.b = Math::lerp(v0->color.b, v1->color.b, t);
	tc.u = Math::lerp(v0->tc.u, v1->tc.u, t);
	tc.v = Math::lerp(v0->tc.v, v1->tc.v, t);
	normal.x = Math::lerp(v0->normal.x, v1->normal.x, t);
	normal.y = Math::lerp(v0->normal.y, v1->normal.y, t);
	normal.z = Math::lerp(v0->normal.z, v1->normal.z, t);
	world_pos.x = Math::lerp(v0->world_pos.x, v1->world_pos.x, t);
	world_pos.y = Math::lerp(v0->world_pos.y, v1->world_pos.y, t);
	world_pos.z = Math::lerp(v0->world_pos.z, v1->world_pos.z, t);
}