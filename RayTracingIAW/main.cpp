#include "sphere.h"
#include "hitablelist.h"
#include "svpng.inc"
#include <float.h>
#include"camera.h"
#include "random.h"


#define W 200
#define H 100  

unsigned char img[W * H * 3];


vec3 color(const ray& r,hitable *world) {
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec)) {
		return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
	}
	else {
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
int main() {
	int nx = W;
	int ny = H;
	int ns = 100;
	hitable *list[2];
	list[0] = new sphere(vec3(0, 0, -1), 0.5);
	list[1] = new sphere(vec3(0, -100.5, -1), 100);
	hitable *world = new hitable_list(list, 2);
	camera cam;
	unsigned char* p = img;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++, p += 3) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r,world);
			}		
			col /= float(ns);
			p[0] = int(255.99*col[0]);
			p[1] = int(255.99*col[1]);
			p[2] = int(255.99*col[2]);
		}
	}

	svpng(fopen("1.png", "wb"), W, H, img, 0);
}