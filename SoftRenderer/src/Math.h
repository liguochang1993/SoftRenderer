#pragma once
#include <cmath>
#include <time.h>

typedef struct { float m[4][4]; } Matrix;
typedef struct { float x, y, z, w; } Vector;
typedef struct { float r, g, b; } Color;
typedef struct { float u, v; } Texcoord;

class Math {

public:
	Math();
	~Math();

	static void matrix_set_identity(Matrix *m);
	static void matrix_set_zero(Matrix *m);
	static void matrix_apply(Vector *y, const Vector *x, const Matrix *m);
	static void matrix_set_perspective(Matrix *m, float fovy, float aspect, float zn, float zf);
	static void matrix_set_orthographic(Matrix *m, float fovy, float aspect, float zn, float zf);
	static void matrix_mul(Matrix *c, const Matrix *a, const Matrix *b);
	static void matrix_set_translate(Matrix *m, float x, float y, float z);
	static void matrix_set_rotate(Matrix *m, float x, float y, float z, float theta);
	static void matrix_set_scale(Matrix *m, float x, float y, float z);
	static void matrix_set_lookat(Matrix *m, const Vector *eye, const Vector *at, const Vector *up, Vector *lookAt);

	static float vector_length(const Vector *v);
	static void vector_sub(Vector *z, const Vector *x, const Vector *y);
	static float vector_dotproduct(const Vector *x, const Vector *y);
	static void vector_crossproduct(Vector *z, const Vector *x, const Vector *y);
	static void vector_normalize(Vector *v);
	static int Clamp(int x, int min, int max);
	static float lerp(float v1, float v2, float t);
	static int fps();
};