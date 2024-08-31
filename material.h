#ifndef MATERIAL_H
#define MATERIAL_H

#include "util.h"

class hitRecord;

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const {
            return false;
        }
};

class Lambertian : public Material {
    public:
        Lambertian(const color& albedo) : albedo(albedo) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            vec3 scatterDir = rec.normal + randomUnitVector();
            if (scatterDir.nearZero()) {
                scatterDir = rec.normal;
            }
            scattered = ray(rec.p, scatterDir);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class Metal : public Material {
    public:
        Metal(const color& albedo, float f) : albedo(albedo), fuzz(f < 1 ? f : 1) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            vec3 reflected = reflect(rIn.direction(), rec.normal);
            reflected = unitVector(reflected) + (fuzz * randomUnitVector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    private:
        color albedo;
        float fuzz;
};

#endif