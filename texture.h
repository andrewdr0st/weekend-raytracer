#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class Texture {
    public:
        virtual ~Texture() = default;

        virtual color value(float u, float v, const point3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(const color& albedo): albedo(albedo) {}

        SolidColor(float r, float g, float b): SolidColor(color(r, g, b)) {}

        color value(float u, float v, const point3& p) const override {
            return albedo;
        }

    private:
        color albedo;
};

class CheckerTexture : public Texture {
    public:
        CheckerTexture(float scale, shared_ptr<Texture> odd, shared_ptr<Texture> even): invScale(1.0 / scale), odd(odd), even(even) {}

        CheckerTexture(float scale, const color& c1, const color& c2): CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

        color value(float u, float v, const point3& p) const override {
            int xInteger = int(std::floor(invScale * p.x()));
            int yInteger = int(std::floor(invScale * p.y()));
            int zInteger = int(std::floor(invScale * p.z()));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        float invScale;
        shared_ptr<Texture> odd;
        shared_ptr<Texture> even;
};

class ImageTexture : public Texture {
    public:
        ImageTexture(const char* filename): image(filename) {}

        color value(float u, float v, const point3& p) const override {
            if(image.height() <= 0) {
                return color(0, 0, 1);
            }

            u = Interval(0,1).clamp(u);
            v = 1.0 - Interval(0,1).clamp(v);

            int i = int(u * image.width());
            int j = int(v * image.height());
            auto pixel = image.pixel_data(i,j);

            float colorScale = 1.0 / 255.0;
            return color(colorScale * pixel[0], colorScale * pixel[1], colorScale * pixel[2]);
        }

    private:
        rtw_image image;
};

class NoiseTexture : public Texture {
    public:
        NoiseTexture(float s): scale(s) {}

        color value(float u, float v, const point3& p) const override {
            return color(0.5, 0.5, 0.5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
        }

    private:
        Perlin noise;
        float scale;
};

#endif