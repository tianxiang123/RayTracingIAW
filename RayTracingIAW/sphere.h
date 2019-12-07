#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2*dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a); //解t   光追步进
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;  //单位化
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;  //单位化
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}


#endif // !SPHEREH
