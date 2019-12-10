#ifndef SPHEREH
#define SPHEREH

#include "hitable.h"
#include "aabb.h"

class moving_sphere :public hitable {
public:
	moving_sphere() {}
	moving_sphere(vec3 cen0, vec3 cen1, float t0, float t1, float r, material *m) :center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center(float time) const;
	vec3 center0, center1;
	float time0, time1;
	float radius;
	material *mat_ptr;
};

vec3 moving_sphere::center(float time) const {
	return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

class sphere : public hitable {
public:
	sphere() {}
	sphere(vec3 cen, float r, material *m) : center(cen), radius(r), mat_ptr(m) {};
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center;
	float radius;
	material *mat_ptr;
};

bool moving_sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec)const { // 运动模糊，运动球体
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = 2 * dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(b*b - 4 * a*c)) / (2 * a); //解t   光追步进
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center(r.time())) / radius;  //单位化
			rec.mat_ptr = mat_ptr;
			return true;
		}
		temp = (-b + sqrt(b*b - 4 * a*c)) / (2 * a);
		if (temp < t_max&&temp>t_min) {
			rec.t = temp;
			rec.pos = r.point_at_parameter(rec.t);
			rec.normal = (rec.pos - center(r.time())) / radius;  //单位化
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}

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

bool sphere::bounding_box(float t0, float t1, aabb& box) const {
	box = aabb(center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius));
	return true;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box)const {
	aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
	aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
	box = surrounding_box(box0, box1);
	return true;
}

#endif // !SPHEREH
