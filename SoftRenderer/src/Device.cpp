#include "device.h"

Device::Device(int w, int h, void *fb) {
	width = w;
	height = h;

	int need = sizeof(void*) * (height * 3 + 1024) + width * height * 12;
	char *ptr = new char[need];

	framebuffer = (unsigned int**)ptr;
	zbuffer = (float**)(ptr + sizeof(void*) * height);
	shadowmap = (float**)(ptr + sizeof(void*) * height * 2);
	ptr += sizeof(void*) * height * 3;

	texture = (unsigned int**)ptr;
	ptr += sizeof(void*) * 1024;

	char *framebuf = new char();
	char *zbuf = new char();
	char *shadow = new char();
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	shadow = (char*)ptr + width * height * 8;
	ptr += width * height * 12;

	if (fb != nullptr)
		framebuf = (char*)fb;

	for (int j = 0; j < height; j++) {
		framebuffer[j] = (unsigned int*)(framebuf + width * 4 * j);
		zbuffer[j] = (float*)(zbuf + width * 4 * j);
		shadowmap[j] = (float*)(shadow + width * 4 * j);
	}

	texture[0] = (unsigned int*)ptr;
	texture[1] = (unsigned int*)(ptr + 16);
	memset(texture[0], 0, 64);
}

Device::~Device() {
	destroy();
}

// 设备初始化
void Device::init(Light *l) {
	tex_width = 2;
	tex_height = 2;
	max_u = 1.0f;
	max_v = 1.0f;
	background = 0xc0c0c0;
	foreground = {0.3f, 0.3f, 0.3f};
	Transform *ts = new Transform(width, height);
	ts->init(0);
	transform = ts;
	light = l;
	init_light();
	render_state = RENDER_STATE_WIREFRAME;
}

// 删除设备
void Device::destroy() {
	if (framebuffer)
		delete[] framebuffer;
	framebuffer = nullptr;
	if (zbuffer)
		delete[] zbuffer;
	zbuffer = nullptr;
	if (shadowmap)
		delete[] shadowmap;
	shadowmap = nullptr;
	if (texture)
		delete[] texture;
	texture = nullptr;
}

// 清空 buffer
void Device::clear() {
	int y, x, h = height;
	for (y = 0; y < height; y++) {
		unsigned int *dst = framebuffer[y];
		unsigned int cc = 0;
		for (x = width; x > 0; dst++, x--) 
			dst[0] = cc;
	}
	for (y = 0; y < height; y++) {
		float *dst = zbuffer[y];
		for (x = width; x > 0; dst++, x--) 
			dst[0] = 0.0f;
	}
	for (y = 0; y < height; y++) {
		float *dst = shadowmap[y];
		for (x = width; x > 0; dst++, x--)
			dst[0] = 0.0f;
	}
}

void Device::init_light() {
	Transform *ts = new Transform(width, height);
	ts->init(1);
	Vector eye = { -light->direction.x * 2, -light->direction.y * 2, -light->direction.z * 2, 1 }, at = { -light->direction.x, -light->direction.y, -light->direction.z, 1 }, up = { 0, 1, 0, 1 };
	light->pos = eye;
	Math::vector_normalize(&light->pos);
	Math::matrix_set_lookat(&ts->view, &eye, &at, &up, &ts->lookAt);
	ts->update();

	light->transform = ts;
	Math::vector_normalize(&light->direction);
}

// 画点
void Device::pixel(int x, int y, unsigned int color) {
	if (((unsigned int)x) < (unsigned int)width && ((unsigned int)y) < (unsigned int)height) {
		framebuffer[y][x] = color;
	}
}

// 画线
void Device::line(Vertex v1, Vertex v2) {
	float x0 = v1.pos.x;
	float y0 = v1.pos.y;
	float x1 = v2.pos.x;
	float y1 = v2.pos.y;

	x0 = (int)(x0 + 0.5f);
	y0 = (int)(y0 + 0.5f);
	x1 = (int)(x1 + 0.5f);
	y1 = (int)(y1 + 0.5f);
	bool flag = false;
	if (std::abs(x0 - x1)<std::abs(y0 - y1)) {
		std::swap(x0, y0);
		std::swap(x1, y1);
		flag = true;
	}
	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}
	int dx = x1 - x0;
	int dy = y1 - y0;
	int d = std::abs(dy);
	int error = 0;
	int y = y0;
	#pragma omp parallel for
	for (int x = x0; x <= (int)x1; x++) {
		float t;
		if (x1 == x0) 
		{
			if (y1 == y0)
			{
				t = 1;
			}
			else
			{
				t = (float)(y - y0) / (y1 - y0);
			}
		}
		else {
			t = (float)(x - x0) / (x1 - x0);
		}

		float z0 = v1.pos.z;
		float z1 = v2.pos.z;
		float z = Math::lerp(z0, z1, t);
		float realz;
		if (z != 0)
		{
			realz = 1.0f / z;
		}
		else
		{
			realz = 0.0f;
		}
		if (is_shadow_mapping)
		{
			shadow_map(x, y, realz);
			continue;
		}
		if (z_test(x, y, realz)) {
			unsigned int color;
			if (render_state & RENDER_STATE_WIREFRAME) {
				float r = foreground.r;
				float g = foreground.g;
				float b = foreground.b;
				int R = (int)(r * 255.0f), G = (int)(g * 255.0f), B = (int)(b * 255.0f);
				R = Math::Clamp(R, 0, 255), G = Math::Clamp(G, 0, 255), B = Math::Clamp(B, 0, 255);
				color = (R << 16) | (G << 8) | (B);
			} 
			else
			{
				Color c0 = v1.color;
				Color c1 = v2.color;
				Texcoord t0 = v1.tc;
				Texcoord t1 = v2.tc;
				Color originColor;
				if (render_state & RENDER_STATE_COLOR)
				{
					float r = Math::lerp(c0.r, c1.r, t) / realz;
					float g = Math::lerp(c0.g, c1.g, t) / realz;
					float b = Math::lerp(c0.b, c1.b, t) / realz;
					originColor = { r, g, b };
				}
				else if (render_state & RENDER_STATE_TEXTURE)
				{
					float u = Math::lerp(t0.u, t1.u, t) / realz;
					float v = Math::lerp(t0.v, t1.v, t) / realz;
					int c = read_texture(u, v);
					float inv = (float)1 / 255;
					originColor = { (c >> 16) * inv, (c >> 8 & 0xff) * inv, (c & 0xff) * inv };
				}

				Vector n0 = v1.normal;
				Vector n1 = v2.normal;
				Vector w0 = v1.world_pos;
				Vector w1 = v2.world_pos;

				float nx = Math::lerp(n0.x, n1.x, t) / realz;
				float ny = Math::lerp(n0.y, n1.y, t) / realz;
				float nz = Math::lerp(n0.z, n1.z, t) / realz;
				Vector normal = { nx, ny, nz, 1.0f};

				float wx = Math::lerp(w0.x, w1.x, t) / realz;
				float wy = Math::lerp(w0.y, w1.y, t) / realz;
				float wz = Math::lerp(w0.z, w1.z, t) / realz;
				Vector world_pos = { wx, wy, wz, 1.0f};

				// 光照

				// 环境光
				Color ambient = light->color;
			
				ambient.r *= light->intensity * originColor.r;
				ambient.g *= light->intensity * originColor.g;
				ambient.b *= light->intensity * originColor.b;

				if (!is_shadow(world_pos))
				{
					// 漫反射
					Color diffuse = light->color;
					float n_dot_d = -Math::vector_dotproduct(&light->direction, &normal);
					if (n_dot_d > 0) {
						diffuse.r *= originColor.r * n_dot_d;
						diffuse.g *= originColor.g * n_dot_d;
						diffuse.b *= originColor.b * n_dot_d;
					}
					else
					{
						diffuse.r = 0.0f;
						diffuse.g = 0.0f;
						diffuse.b = 0.0f;
					}

					ambient.r += diffuse.r;
					ambient.g += diffuse.g;
					ambient.b += diffuse.b;

					// 镜面光
					Color specular = light->color;
					Vector view;
					Math::vector_sub(&view, &camera_pos, &world_pos);
					Math::vector_normalize(&view);
					Vector reflect;
					reflect.x = (light->direction.x + view.x) * light->specularStrength;
					reflect.y = (light->direction.y + view.y) * light->specularStrength;
					reflect.z = (light->direction.z + view.z) * light->specularStrength;
					float n_dot_s = Math::vector_dotproduct(&normal, &reflect);
					float pf = 0 ? 0 : pow(n_dot_s, 32);

					specular.r *= originColor.r * pf;
					specular.g *= originColor.g * pf;
					specular.b *= originColor.b * pf;

					ambient.r += specular.r;
					ambient.g += specular.g;
					ambient.b += specular.b;
				}

				int R = (int)(ambient.r * 255.0f), G = (int)(ambient.g * 255.0f), B = (int)(ambient.b * 255.0f);
				R = Math::Clamp(R, 0, 255), G = Math::Clamp(G, 0, 255), B = Math::Clamp(B, 0, 255);
				color = (R << 16) | (G << 8) | (B);
			}
			if (flag) {
				pixel(y, x, color);
			}
			else {
				pixel(x, y, color);
			}
		}
		error += d;
		if (error > dx) {
			y += (y1 > y0 ? 1 : -1);
			error -= dx;
		}
	}
}

// 画图元三角形
void Device::triangle(Vertex *v1, Vertex *v2, Vertex *v3) {
	is_shadow_mapping = true;
	render(v1, v2, v3, light->transform);
	is_shadow_mapping = false;
	render(v1, v2, v3, transform);
}

void Device::render(Vertex *v1, Vertex *v2, Vertex *v3, Transform *ts) {
	Vector p1, p2, p3, c1, c2, c3;
	c1 = v1->pos;
	c2 = v2->pos;
	c3 = v3->pos;
	int state = render_state;

	v1->world_pos = c1;
	v2->world_pos = c2;
	v3->world_pos = c3;

	ts->set_normal(v1, v2, v3);

	// 按照 Transform 变化
	ts->apply(&c1, &v1->pos);
	ts->apply(&c2, &v2->pos);
	ts->apply(&c3, &v3->pos);

	// 裁剪
	if (!ts->check_cvv(&c1)) return;
	if (!ts->check_cvv(&c2)) return;
	if (!ts->check_cvv(&c3)) return;

	// 归一化
	ts->homogenize(&p1, &c1);
	ts->homogenize(&p2, &c2);
	ts->homogenize(&p3, &c3);

	// 背面剔除
	if (ts->check_back_culling(&p1, &p2, &p3))
		return;

	Vertex t1 = *v1, t2 = *v2, t3 = *v3;
	t1.pos = p1;
	t2.pos = p2;
	t3.pos = p3;
	t1.pos.w = c1.w;
	t2.pos.w = c2.w;
	t3.pos.w = c3.w;

	// 纹理、色彩绘制
	if (render_state & (RENDER_STATE_TEXTURE | RENDER_STATE_COLOR)) {
		t1.rhw_init();
		t2.rhw_init();
		t3.rhw_init();
		triangle_raster(&t1, &t2, &t3);
	}

	// 线框绘制
	if (state & RENDER_STATE_WIREFRAME) {
		line(t1, t2);
		line(t1, t3);
		line(t3, t2);
	}
}

void Device::triangle_raster(Vertex *v0, Vertex *v1, Vertex *v2) {
	if (v1->pos.y <= v0->pos.y)
	{
		if ((v1->pos.y != v0->pos.y) || (v1->pos.x < v0->pos.x))
			std::swap(v0 , v1);
	}
	if (v2->pos.y <= v0->pos.y)
	{
		if ((v2->pos.y != v0->pos.y) || (v2->pos.x < v0->pos.x))
			std::swap(v0, v2);
	}
	if (v2->pos.y <= v1->pos.y)
	{
		if ((v2->pos.y != v1->pos.y) || (v2->pos.x < v1->pos.x))
			std::swap(v1, v2);
	}

	if (v0->pos.y == v1->pos.y)		//平顶三角形
	{
		top_trangle_raster(v0, v1, v2);
	}
	else if (v1->pos.y == v2->pos.y)	//平底三角形
	{
		bottom_trangle_raster(v0, v1, v2);
	}
	else	 //平顶三角形和平底三角形
	{
		float x3 = (v1->pos.y - v0->pos.y) * (v2->pos.x - v0->pos.x) / (v2->pos.y - v0->pos.y) + v0->pos.x;
		float y3 = v1->pos.y;

		bool f = false;
		Vertex *v3 = new Vertex({ x3, y3, 0.0f }, { 0.0f, 0.0f, 0.0f });

		float t3 = (float)(y3 - v0->pos.y) / (v2->pos.y - v0->pos.y);
		v3->lerp(v0, v2, t3);

		if (v1->pos.x > x3)
		{
			std::swap(v1, v3);
			f = true;
		}
		
		bottom_trangle_raster(v0, v1, v3);
		top_trangle_raster(v1, v3, v2);

		if (f)
			delete v1;
		else
			delete v3;
	}
}

void Device::bottom_trangle_raster(Vertex *v0, Vertex *v1, Vertex *v2) {
	for (float y = v0->pos.y; y <= v1->pos.y; y++)
	{
		float xl = (y - v1->pos.y) * (v0->pos.x - v1->pos.x) / (v0->pos.y - v1->pos.y) + v1->pos.x;
		float xr = (y - v2->pos.y) * (v0->pos.x - v2->pos.x) / (v0->pos.y - v2->pos.y) + v2->pos.x;

		Vertex *vl = new Vertex({ xl, y, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Vertex *vr = new Vertex({ xr, y, 0.0f }, { 0.0f, 0.0f, 0.0f });

		float tl = (float)(y - v1->pos.y) / (v0->pos.y - v1->pos.y);
		vl->lerp(v1, v0, tl);

		float tr = (float)(y - v2->pos.y) / (v0->pos.y - v2->pos.y);
		vr->lerp(v2, v0, tr);
		line(*vl, *vr);
		delete vl;
		delete vr;
	}
}

void Device::top_trangle_raster(Vertex *v0, Vertex *v1, Vertex *v2) {
	for (float y = v0->pos.y; y <= v2->pos.y; y++)
	{
		float xl = (y - v0->pos.y) * (v2->pos.x - v0->pos.x) / (v2->pos.y - v0->pos.y) + v0->pos.x;
		float xr = (y - v1->pos.y) * (v2->pos.x - v1->pos.x) / (v2->pos.y - v1->pos.y) + v1->pos.x;

		Vertex *vl = new Vertex({ xl, y, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Vertex *vr = new Vertex({ xr, y, 0.0f }, { 0.0f, 0.0f, 0.0f });

		float tl = (float)(y - v0->pos.y) / (v2->pos.y - v0->pos.y);
		vl->lerp(v0, v2, tl);

		float tr = (float)(y - v1->pos.y) / (v2->pos.y - v1->pos.y);
		vr->lerp(v1, v2, tr);
		line(*vl, *vr);
		delete vl;
		delete vr;
	}
}

bool Device::z_test(int x, int y, float depth)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		if (zbuffer[y][x] < depth)
		{
			zbuffer[y][x] = depth;
			return true;
		}
	}
	return false;
}

void Device::shadow_map(int x, int y, float depth)
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		if (shadowmap[y][x] < depth)
		{
			shadowmap[y][x] = depth;
		}
	}
}

bool Device::is_shadow(Vector v)
{
	Vector vl;
	Vector vf;
	light->transform->apply(&vl, &v);
	light->transform->homogenize(&vf, &vl);

	int x = (int)(vf.x + 0.5f);
	int y = (int)(vf.y + 0.5f);

	float depth;
	if (vf.z != 0)
	{
		depth = 1.0f / vf.z;
	}
	else
	{
		depth = 0.0f;
	}

	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		if (shadowmap[y][x] -0.05 > depth)
		{
			return true;
		}
	}
	return false;
}

void Device::load_texture(const char* path, const int width, const int height) {
	int w = width, h = height;
	int p = w * 4;
	static unsigned int tex[350][350];

	HBITMAP bitmap = (HBITMAP)LoadImage(NULL, path, IMAGE_BITMAP, w, h, LR_LOADFROMFILE);

	HDC hdc = CreateCompatibleDC(NULL);
	SelectObject(hdc, bitmap);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			COLORREF color = GetPixel(hdc, i, j);
			int R = color % 256;
			int G = (color >> 8) % 256;
			int B = (color >> 16) % 256;
			tex[i][j] = (R << 16) | (G << 8) | (B);
		}
	}

	char *ptr = (char*)tex;
	for (int j = 0; j < h; ptr += p, j++)
	{
		texture[j] = (unsigned int*)ptr;
	}
	tex_width = w;
	tex_height = h;
	max_u = (float)(w - 1);
	max_v = (float)(h - 1);
}

unsigned int Device::read_texture(float u, float v) {
	int x, y;
	u = u * max_u;
	v = v * max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = Math::Clamp(x, 0, tex_width - 1);
	y = Math::Clamp(y, 0, tex_height - 1);
	return texture[y][x];
}