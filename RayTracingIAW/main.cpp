#include "sphere.h"
#include "hitablelist.h"
#include "svpng.inc"
#include <float.h>
#include"camera.h"
#include "random.h"
#include "material.h"
#include <omp.h>


#define W 2000
#define H 1000

unsigned char img[W * H * 3];

//vec3 random_in_unit_sphere() {
//	vec3 p;
//	do {
//		p = 2.0*vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1); //2,2,2������
//	} while (p.squared_length() >= 1.0); //ȡ�ᣬģ��һ��BRDF
//	return p;
//}

vec3 color(const ray& r,hitable *world,int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}	
	}
	else
	{
		vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5*(unit_direction.y() + 1.0);
		return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}
int main() {
	int nx = W;
	int ny = H;
	int ns = 100;
	hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2),0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);

	camera cam;
	unsigned char* p = img;

#ifdef DEBUG
#else
#pragma omp parallel for schedule(dynamic,3)
	//#pragma omp parallel for num_threads(12)
#endif
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0, 0, 0);
			for (int s = 0; s < ns; s++) {
				float u = float(i + random_double()) / float(nx);
				float v = float(j + random_double()) / float(ny);
				ray r = cam.get_ray(u, v);
				col += color(r,world,0);
			}		
			col /= float(ns);
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

			p[(ny - 1 - j)* nx * 3 + i * 3 + 0] = int(255.99*col[0]);
			p[(ny - 1 - j)* nx * 3 + i * 3 + 1] = int(255.99*col[1]);
			p[(ny - 1 - j)* nx * 3 + i * 3 + 2] = int(255.99*col[2]);
		}
	}
	svpng(fopen("dielectric3.png", "wb"), W, H, img, 0);
}