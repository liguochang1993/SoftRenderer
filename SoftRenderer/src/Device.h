#pragma once
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <omp.h>
#include "Define.h"
#include "Transform.h"
#include "Vertex.h"
#include "Light.h"
#include "Math.h"

class Device {
public:
	Device(int w, int h, void *fb);
	~Device();

	Transform *transform;		// ����任��
	Light *light;				// ����
	int width;                  // ���ڿ��
	int height;                 // ���ڸ߶�
	unsigned int **framebuffer;	// ���ػ���
	float **zbuffer;            // ��Ȼ���
	float **shadowmap;          // ���ͼ
	unsigned int **texture;     // ����ͬ����ÿ������
	int tex_width;              // ������
	int tex_height;             // ����߶�
	float max_u;                // ��������ȣ�tex_width - 1
	float max_v;                // �������߶ȣ�tex_height - 1
	int render_state;           // ��Ⱦ״̬
	unsigned int background;    // ������ɫ
	Color foreground;			// �߿���ɫ
	Vector camera_pos;			// �����λ��
	bool is_shadow_mapping = false;

	void init(Light *l);
	void destroy();
	void clear();
	void init_light();
	void pixel(int x, int y, unsigned int color);
	void line(Vertex v1, Vertex v2);
	void triangle(Vertex *v1, Vertex *v2, Vertex *v3);
	void render(Vertex *v1, Vertex *v2, Vertex *v3, Transform *transform);
	void triangle_raster(Vertex *v1, Vertex *v2, Vertex *v3);
	void bottom_trangle_raster(Vertex *v1, Vertex *v2, Vertex *v3);
	void top_trangle_raster(Vertex *v1, Vertex *v2, Vertex *v3);
	bool z_test(int x, int y, float depth);
	void shadow_map(int x, int y, float depth);
	bool is_shadow(Vector v);
	void load_texture(const char* path, int width, int height);
	unsigned int read_texture(float u, float v);
};
