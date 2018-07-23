#include <iostream>
#include <Windows.h>
#include "Screen.h"
#include "Device.h"
#include "Vertex.h"
#include "Light.h"
#include "Math.h"

Vertex mesh_lerp[8] = {
	{ { 1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, -1, 1, 1 },{ 0.2f, 1.0f, 0.2f } },
	{ { -1, 1, 1, 1 },{ 0.2f, 0.2f, 1.0f } },
	{ { 1, 1, 1, 1 },{ 1.0f, 0.2f, 1.0f } },
	{ { 1, -1, -1, 1 },{ 1.0f, 1.0f, 0.2f } },
	{ { -1, -1, -1, 1 },{ 0.2f, 1.0f, 1.0f } },
	{ { -1, 1, -1, 1 },{ 1.0f, 0.3f, 0.3f } },
	{ { 1, 1, -1, 1 },{ 0.2f, 1.0f, 0.3f } },
};

Vertex mesh_1[8] = {
	{ { 1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, 1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, 1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, 1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, 1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
};

Vertex mesh_2[8] = {
	{ { 4, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 2, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 2, 1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 4, 1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 4, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 2, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 2, 1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 4, 1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
};

Vertex mesh_plane_1[4] = {
	{ { 1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
};
Vertex mesh_plane_2[4] = {
	{ { 1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, 3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, 3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
};
Vertex mesh_plane_3[4] = {
	{ { 1, -1, -3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 3, -1, -3, 1 },{ 1.0f, 0.2f, 0.2f } },
};
Vertex mesh_plane_4[4] = {
	{ { -1, -1, -3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, -1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, -3, 1 },{ 1.0f, 0.2f, 0.2f } },
};
Vertex mesh_plane_5[4] = {
	{ { -1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { -1, -1, 3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, 3, 1 },{ 1.0f, 0.2f, 0.2f } },
	{ { 1, -1, 1, 1 },{ 1.0f, 0.2f, 0.2f } },
};

void draw_plane(Device *device, Vertex *mesh, int a, int b, int c, int d) {
	Vertex p1 = mesh[a], p2 = mesh[b], p3 = mesh[c], p4 = mesh[d];
	p1.tc.u = 0, p1.tc.v = 0, p2.tc.u = 0, p2.tc.v = 1;
	p3.tc.u = 1, p3.tc.v = 1, p4.tc.u = 1, p4.tc.v = 0;
	device->triangle(&p1, &p2, &p3);
	device->triangle(&p3, &p4, &p1);
}

void draw_box(Device *device, Vertex *mesh) {
	draw_plane(device, mesh, 0, 1, 2, 3);
	draw_plane(device, mesh, 7, 6, 5, 4);
	draw_plane(device, mesh, 0, 4, 5, 1);
	draw_plane(device, mesh, 1, 5, 6, 2);
	draw_plane(device, mesh, 2, 6, 7, 3);
	draw_plane(device, mesh, 3, 7, 4, 0);
}

void camera_init(Device *device, float x, float y, float z) {
	Vector eye = { x, y, z, 1 }, at = { 0, 0, 0, 1 }, up = { 0, 1, 0, 1 };
	device->camera_pos = eye;
	Math::vector_normalize(&device->camera_pos);
	Math::matrix_set_lookat(&device->transform->view, &eye, &at, &up, &device->transform->lookAt);
	device->transform->update();
}

int main(int argc, char** argv) {
	LPCTSTR title = LPCTSTR("Soft Renderer");
	Screen *screen = new Screen(800, 600, title);

	if (screen->init())
		return -1;

	omp_set_num_threads(4);
	Light *light = new Light({ 3.0f, -6.0f, 3.0f, 1.0f }, { 1.0f, 1.0f, 1.0f });
	Device *device = new Device(800, 600, screen->fb);
	device->init(light);
	camera_init(device, 6, 6, -3);
	device->render_state = RENDER_STATE_COLOR;
	//device->render_state = RENDER_STATE_TEXTURE;

	device->load_texture("res\\geralt.bmp", 350, 350);

	while (screen->getExit() == 0 && screen->getKeys()[VK_ESCAPE] == 0) {
		screen->dispatch();
		device->clear();

		// 位移测试
		/*Matrix sm;
		Math::matrix_set_scale(&sm, 1.5f, 1.5f, 1.5f);
		Matrix rm;
		Math::matrix_set_rotate(&rm, 0.0f, 1.0f, 0.0f, 1.0f);
		Matrix tm;
		Math::matrix_set_translate(&tm, 0.0f, 0.0f, 3.0f);
		Matrix m;
		Math::matrix_mul(&m, &rm, &tm);
		Math::matrix_mul(&m, &sm, &m);
		device->transform->world = m;
		device->transform->update();*/
		
		// 三角形
		/*Vertex *v1 = new Vertex({ 2, -1, 1, 1 }, { 0.0f, 0.0f, 0.0f });
		Vertex *v2 = new Vertex({ -1, -2, 1, 1 }, { 0.5f, 0.5f, 0.5f });
		Vertex *v3 = new Vertex({ -1, 1, 1, 1 }, { 1.0f, 1.0f, 1.0f });
		device->triangle(v1, v2, v3);*/

		// 正方体
		draw_box(device, mesh_1);
		//draw_box(device, mesh_2);
		//draw_box(device, mesh_lerp);

		// 地面
		draw_plane(device, mesh_plane_1, 3, 2, 1, 0);
		draw_plane(device, mesh_plane_2, 3, 2, 1, 0);
		draw_plane(device, mesh_plane_3, 3, 2, 1, 0);
		draw_plane(device, mesh_plane_4, 3, 2, 1, 0);
		draw_plane(device, mesh_plane_5, 3, 2, 1, 0);

		screen->update();

		//std::cout << Math::fps() << std::endl;
	}

	return 0;
}