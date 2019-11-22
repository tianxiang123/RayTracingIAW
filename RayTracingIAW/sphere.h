#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r) : center(cen), radius(r) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	vec3 center;
	float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const {
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = 2*dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a); //��t   ��׷����
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;  //��λ��
			return true;
		}
		temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center) / radius;  //��λ��
			return true;
		}
	}
	return false;
}


#endif // !SPHEREH
