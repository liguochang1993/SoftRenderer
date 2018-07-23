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

	Transform *transform;		// 坐标变换器
	Light *light;				// 光照
	int width;                  // 窗口宽度
	int height;                 // 窗口高度
	unsigned int **framebuffer;	// 像素缓存
	float **zbuffer;            // 深度缓存
	float **shadowmap;          // 深度图
	unsigned int **texture;     // 纹理：同样是每行索引
	int tex_width;              // 纹理宽度
	int tex_height;             // 纹理高度
	float max_u;                // 纹理最大宽度：tex_width - 1
	float max_v;                // 纹理最大高度：tex_height - 1
	int render_state;           // 渲染状态
	unsigned int background;    // 背景颜色
	Color foreground;			// 线框颜色
	Vector camera_pos;			// 摄像机位置
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
