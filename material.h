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

class Dielectric : public Material {
    public:
        Dielectric(float ri) : refractionIndex(ri) {}

        bool scatter(const ray& rIn, const hitRecord& rec, color& attenuation, ray& scattered) const override {
            attenuation = color(1.0, 1.0, 1.0);
            float ri = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

            vec3 unitDir = unitVector(rIn.direction());
            float cosTheta = std::fmin(dot(-unitDir, rec.normal), 1.0);
            float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

            bool cannotRefract = ri * sinTheta > 1.0;
            vec3 dir;

            if (cannotRefract || reflectance(cosTheta, ri) > randomFloat()) {
                dir = reflect(unitDir, rec.normal);
            } else {
                dir = refract(unitDir, rec.normal, ri);
            }

            scattered = ray(rec.p, dir);
            return true;
        }

    private:
        float refractionIndex;

        static float reflectance(float cosine, float ri) {
            float r0 = (1 - ri) / (1 + ri);
            r0 = r0 * r0;
            return r0 + (1 - r0) * std::pow((1 - cosine), 5);
        }
};

#endif