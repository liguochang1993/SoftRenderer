#include "Math.h"

Math::Math() {
}

Math::~Math() {}

void Math::matrix_set_identity(Matrix *m) {
	m->m[0][0] = m->m[1][1] = m->m[2][2] = m->m[3][3] = 1.0f;
	m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
}

void Math::matrix_set_zero(Matrix *m) {
	m->m[0][0] = m->m[0][1] = m->m[0][2] = m->m[0][3] = 0.0f;
	m->m[1][0] = m->m[1][1] = m->m[1][2] = m->m[1][3] = 0.0f;
	m->m[2][0] = m->m[2][1] = m->m[2][2] = m->m[2][3] = 0.0f;
	m->m[3][0] = m->m[3][1] = m->m[3][2] = m->m[3][3] = 0.0f;
}

void Math::matrix_apply(Vector *y, const Vector *x, const Matrix *m)
{
	float X = x->x, Y = x->y, Z = x->z, W = x->w;
	y->x = X * m->m[0][0] + Y * m->m[1][0] + Z * m->m[2][0] + W * m->m[3][0];
	y->y = X * m->m[0][1] + Y * m->m[1][1] + Z * m->m[2][1] + W * m->m[3][1];
	y->z = X * m->m[0][2] + Y * m->m[1][2] + Z * m->m[2][2] + W * m->m[3][2];
	y->w = X * m->m[0][3] + Y * m->m[1][3] + Z * m->m[2][3] + W * m->m[3][3];
}

void Math::matrix_set_perspective(Matrix *m, float fovy, float aspect, float zn, float zf) {
	float fax = 1.0f / (float)tan(fovy * 0.5f);
	matrix_set_zero(m);
	m->m[0][0] = (float)(fax / aspect);
	m->m[1][1] = (float)(fax);

	//DX
	m->m[2][2] = zf / (zf - zn);
	m->m[3][2] = zn * zf / (zn - zf);
	m->m[2][3] = 1;

	//OpenGL
	/*m->m[2][2] = (zn + zf) / (zn - zf);
	m->m[3][2] = -1;
	m->m[2][3] = 2.0f * zn * zf / (zn - zf);*/
}

void Math::matrix_set_orthographic(Matrix *m, float width, float height, float zn, float zf) {
	matrix_set_zero(m);
	m->m[0][0] = (float)(2 / width);
	m->m[1][1] = (float)(2 / height);

	//DX
	m->m[2][2] = 1 / (zf - zn);
	m->m[3][2] = zn / (zn - zf);
	m->m[3][3] = 1;
}

void Math::matrix_mul(Matrix *c, const Matrix *a, const Matrix *b) {
	Matrix mat;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			mat.m[i][j] = (a->m[i][0] * b->m[0][j]);
			mat.m[i][j] += (a->m[i][1] * b->m[1][j]);
			mat.m[i][j] += (a->m[i][2] * b->m[2][j]);
			mat.m[i][j] += (a->m[i][3] * b->m[3][j]);
		}
	}
	c[0] = mat;
}

// translate
void Math::matrix_set_translate(Matrix *m, float x, float y, float z)
{
	Math::matrix_set_identity(m);
	m->m[3][0] = x;
	m->m[3][1] = y;
	m->m[3][2] = z;
}

// rotate
//void Math::matrix_set_rotate(Matrix *m, float x, float y, float z, float theta)
//{
//	float qsin = (float)sin(theta * 0.5f);
//	float qcos = (float)cos(theta * 0.5f);
//	Vector vec = { x,y,z,1.0f };
//	float w = qcos;
//	Math::vector_normalize(&vec);
//	x = vec.x * qsin;
//	y = vec.y * qsin;
//	z = vec.z * qsin;
//	m->m[0][0] = 1 - 2 * y * y - 2 * z * z;
//	m->m[1][0] = 2 * x * y - 2 * w * z;
//	m->m[2][0] = 2 * x * z + 2 * w * y;
//	m->m[0][1] = 2 * x * y + 2 * w * z;
//	m->m[1][1] = 1 - 2 * x * x - 2 * z * z;
//	m->m[2][1] = 2 * y * z - 2 * w * x;
//	m->m[0][2] = 2 * x * z - 2 * w * y;
//	m->m[1][2] = 2 * y * z - 2 * w * x;
//	m->m[2][2] = 1 - 2 * x * x - 2 * y * y;
//	m->m[0][3] = m->m[1][3] = m->m[2][3] = 0.0f;
//	m->m[3][0] = m->m[3][1] = m->m[3][2] = 0.0f;
//	m->m[3][3] = 1.0f;
//}
void Math::matrix_set_x_rotate(Matrix *m, float theta)
{
	Math::matrix_set_identity(m);
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	m->m[1][1] = qcos;
	m->m[1][2] = qsin;
	m->m[2][1] = -qsin;
	m->m[2][2] = qcos;
}

void Math::matrix_set_y_rotate(Matrix *m, float theta)
{
	Math::matrix_set_identity(m);
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	m->m[0][0] = qcos;
	m->m[0][2] = -qsin;
	m->m[2][0] = qsin;
	m->m[2][2] = qcos;
}

void Math::matrix_set_z_rotate(Matrix *m, float theta)
{
	Math::matrix_set_identity(m);
	float qsin = (float)sin(theta * 0.5f);
	float qcos = (float)cos(theta * 0.5f);
	m->m[0][0] = qcos;
	m->m[0][1] = qsin;
	m->m[1][0] = -qsin;
	m->m[1][1] = qcos;
}

// scale
void Math::matrix_set_scale(Matrix *m, float x, float y, float z)
{
	Math::matrix_set_identity(m);
	m->m[0][0] = x;
	m->m[1][1] = y;
	m->m[2][2] = z;
}

void Math::matrix_set_lookat(Matrix *m, const Vector *eye, const Vector *at, const Vector *up, Vector *lookAt) {
	Vector xaxis, yaxis, zaxis;

	// DX
	vector_sub(&zaxis, at, eye);
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	m->m[0][0] = xaxis.x;
	m->m[1][0] = xaxis.y;
	m->m[2][0] = xaxis.z;
	m->m[3][0] = -vector_dotproduct(&xaxis, eye);

	m->m[0][1] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[2][1] = yaxis.z;
	m->m[3][1] = -vector_dotproduct(&yaxis, eye);

	m->m[0][2] = zaxis.x;
	m->m[1][2] = zaxis.y;
	m->m[2][2] = zaxis.z;
	m->m[3][2] = -vector_dotproduct(&zaxis, eye);

	//OpenGL
	/*vector_sub(&zaxis, eye, at);
	vector_normalize(&zaxis);
	vector_crossproduct(&xaxis, up, &zaxis);
	vector_normalize(&xaxis);
	vector_crossproduct(&yaxis, &zaxis, &xaxis);

	m->m[0][0] = xaxis.x;
	m->m[0][1] = xaxis.y;
	m->m[0][2] = xaxis.z;
	m->m[0][3] = -vector_dotproduct(&xaxis, eye);

	m->m[1][0] = yaxis.x;
	m->m[1][1] = yaxis.y;
	m->m[1][2] = yaxis.z;
	m->m[1][3] = -vector_dotproduct(&yaxis, eye);

	m->m[2][0] = -zaxis.x;
	m->m[2][1] = -zaxis.y;
	m->m[2][2] = -zaxis.z;
	m->m[2][3] = vector_dotproduct(&zaxis, eye);*/

	lookAt = &zaxis;
}

float Math::vector_length(const Vector *v) {
	float m = v->x * v->x + v->y * v->y + v->z * v->z;
	return (float)sqrt(m);
}

// z = x - y
void Math::vector_sub(Vector *z, const Vector *x, const Vector *y) {
	z->x = x->x - y->x;
	z->y = x->y - y->y;
	z->z = x->z - y->z;
	z->w = 1.0f;
}

// 矢量点乘
float Math::vector_dotproduct(const Vector *x, const Vector *y) {
	return x->x * y->x + x->y * y->y + x->z * y->z;
}

// 矢量叉乘
void Math::vector_crossproduct(Vector *z, const Vector *x, const Vector *y) {
	z->x = x->y * y->z - x->z * y->y;
	z->y = x->z * y->x - x->x * y->z;
	z->z = x->x * y->y - x->y * y->x;
}

// 矢量归一化
void Math::vector_normalize(Vector *v) {
	float len = vector_length(v);
	if (len != 0.0f) {
		float n = 1.0f / len;
		v->x *= n;
		v->y *= n;
		v->z *= n;
	}
}

// 限制范围
int Math::Clamp(int x, int min, int max) {
	return (x < min) ? min : ((x > max) ? max : x);
}

// 插值
float Math::lerp(float v1, float v2, float t) {
	return v1 + (v2 - v1) * t; 
}

int Math::fps() {
	static int fps = 0;
	static int lastTime = clock();
	static int frameCount = 0;

	++frameCount;

	int curTime = clock();
	if (curTime - lastTime > 1000)
	{
		fps = frameCount;
		frameCount = 0;
		lastTime = curTime;
	}
	return fps;
}
