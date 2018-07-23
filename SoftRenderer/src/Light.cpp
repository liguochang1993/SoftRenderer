#include "Light.h"

Light::Light(Vector d, Color c) {
	direction = d;
	color = c;
}

Light::~Light() {}