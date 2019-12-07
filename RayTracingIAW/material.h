#ifndef MATERIALH
#define MATERIALH
#include "ray.h"
#include "hitable.h"

//struct hit_record;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation,ray& scattered) const = 0;
};

vec3 random_in_unit_sphere() {
	vec3 p;
	do {
		p = 2.0*vec3(random_double(), random_double(), random_double()) - vec3(1, 1, 1);
	} while (p.squared_length() >= 1.0);
	return p;
}

vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n)*n;
};

//bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
//	vec3 uv = unit_vector(v);
//	float dt = dot(uv, n);
//	float discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
//
//}

class lambertian : public material {
public:
	lambertian(const vec3& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec,
						vec3& attenuation, ray& scattered) const {
		vec3 target = rec.pos + rec.normal + random_in_unit_sphere();
		scattered = ray(rec.pos, target - rec.pos);
		attenuation = albedo;
		return true;
	}

	vec3 albedo;
};

class metal : public material {
public:
	metal(const vec3& a, float f) : albedo(a) {
		if (f < 1) fuzz = f;
		else fuzz = 1;
	}
	virtual bool scatter(const ray& r_in, const hit_record& rec,
		vec3& attenuation, ray& scattered) const {
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.pos, reflected);
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	vec3 albedo;
	float fuzz;
};



#endif // !MATERIALH

