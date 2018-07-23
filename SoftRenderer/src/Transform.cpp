#include "Transform.h"

Transform::Transform(int width, int height) {
	w = width;
	h = height;
}

Transform::~Transform() {}

void Transform::init(int pro_type) {
	float aspect = (float)w / ((float)h);
	Math::matrix_set_identity(&world);
	Math::matrix_set_identity(&view);
	if (pro_type == 0)
	{
		Math::matrix_set_perspective(&projection, 3.1415926f * 0.5f, aspect, 1.0f, 500.0f);
	}
	else
	{
		Math::matrix_set_orthographic(&projection, 12.0f, 8.0f, 0.1f, 100.0f);
	}
	
	update();
}

void Transform::update() {
	Matrix m;

	// DX
	Math::matrix_mul(&m, &world, &view);
	Math::matrix_mul(&transform, &m, &projection);

	// OpenGL
	/*Math::matrix_mul(&m, &projection, &view);
	Math::matrix_mul(&transform, &world, &m);*/
}

void Transform::apply(Vector *y, const Vector *x) {
	Math::matrix_apply(y, x, &transform);
}

bool Transform::check_cvv(const Vector *v) {
	float w = v->w;
	bool check = true;
	// DX
	if (v->z < 0.0f) check = false;
	// OpenGL
	//if (v->z < -w) check = false;
	if (v->z > w) check = false;
	if (v->x < -w) check = false;
	if (v->x > w) check = false;
	if (v->y < -w) check = false;
	if (v->y > w) check = false;
	return check;
}

void Transform::homogenize(Vector *y, const Vector *x) {
	float rhw = 1.0f / x->w;
	y->x = (x->x * rhw + 1.0f) * w * 0.5f;

	// DX
	y->y = (1.0f - x->y * rhw) * h * 0.5f;

	// OpenGL
	//y->y = (x->y * rhw + 1.0f) * h * 0.5f;

	y->z = x->z * rhw;
	y->w = 1.0f;
}

// 计算法向量
void Transform::set_normal(Vertex *v1, Vertex *v2, Vertex *v3)
{
	Vector s1, s2, p;
	Math::vector_sub(&s1, &v2->pos, &v1->pos);
	Math::vector_sub(&s2, &v3->pos, &v2->pos);
	Math::vector_crossproduct(&p, &s2, &s1);
	Math::vector_normalize(&p);

	p.w = 0.0f;

	v1->normal = p;
	v2->normal = p;
	v3->normal = p;
}


bool Transform::check_back_culling(const Vector *v1, const Vector *v2, const Vector *v3) {
	Vector s1, s2, p;
	Math::vector_sub(&s1, v2, v1);
	Math::vector_sub(&s2, v3, v2);
	Math::vector_crossproduct(&p, &s2, &s1);
	Math::vector_normalize(&p);
	float dot = Math::vector_dotproduct(&lookAt, &p);
	if (dot > 0) {
		return true;
	}
	return false;
}