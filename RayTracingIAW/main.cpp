#include "ray.h"
#include "svpng.inc"

#define W 200
#define H 100

unsigned char img[W * H * 3];

float hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2.0*dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4*a*c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - sqrt(discriminant)) / (2.0*a);  //�������t����СֵΪ�״��ཻ��
	}
}

vec3 color(const ray& r) {
	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1)); //������Բ���γɵĵ�λ����
		return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1); //  ��(-1,1)ת����(0,1)����
	}
	vec3 unit_direction = unit_vector(r.direction());
	t = 0.5*(unit_direction.y() + 1.0);
	return (1.0 - t)*vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}
int main() {
	int nx = W;
	int ny = H;
	vec3 lower_left_corner(-2.0, -1.0, -1.0);
	vec3 horizontal(4.0, 0.0, 0.0);
	vec3 vertical(0.0, 2.0, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	unsigned char* p = img;
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++, p += 3) {
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);
			vec3 col = color(r);
			p[0] = int(255.99*col[0]);
			p[1] = int(255.99*col[1]);
			p[2] = int(255.99*col[2]);
		}
	}
	svpng(fopen("1.png", "wb"), W, H, img, 0);
}