#pragma once
#include <iostream>
#include "Vertex.h"
#include "Math.h"

class Transform {

public:
	Transform(int width, int height);
	~Transform();

	Vector lookAt;

	Matrix world;         // ��������任
	Matrix view;          // ��Ӱ������任
	Matrix projection;    // ͶӰ�任
	Matrix transform;     // transform = world * view * projection
	float w, h;

	void init(int pro_type);
	void update();
	void apply(Vector *y, const Vector *x);
	void homogenize(Vector *y, const Vector *x);
	bool check_cvv(const Vector *v);
	void set_normal(Vertex *v1, Vertex *v2, Vertex *v3);
	bool check_back_culling(const Vector *v1, const Vector *v2, const Vector *v3);
};