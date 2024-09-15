#ifndef PERLIN_H
#define PERLIN_H

#include "util.h"

class Perlin {
    public:
        Perlin() {
            for (int i = 0; i < pointCount; i++) {
                randVec[i] = unitVector(vec3::random(-1, 1));
            }
            perlinGeneratePerm(permX);
            perlinGeneratePerm(permY);
            perlinGeneratePerm(permZ);
        }

        float noise(const point3& p) const {
            float u = p.x() - std::floor(p.x());
            float v = p.y() - std::floor(p.y());
            float w = p.z() - std::floor(p.z());

            int i = std::floor(p.x());
            int j = std::floor(p.y());
            int k = std::floor(p.z());
            vec3 c[2][2][2];

            for (int di = 0; di < 2; di++) {
                for (int dj = 0; dj < 2; dj++) {
                    for (int dk = 0; dk < 2; dk++) {
                        c[di][dj][dk] = randVec[permX[(i + di) & 255] ^ permY[(j + dj) & 255] ^ permZ[(k + dk) & 255]];
                    }
                }
            }

            return trilinearInterpolation(c, u, v, w);
        }

        float turb(const point3& p, int depth) const {
            float accum = 0.0;
            point3 tempP = p;
            float weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(tempP);
                weight *= 0.5;
                tempP *= 2;
            }

            return std::fabs(accum);
        }

    private:
        static const int pointCount = 256;
        vec3 randVec[pointCount];
        int permX[pointCount];
        int permY[pointCount];
        int permZ[pointCount];

        static void perlinGeneratePerm(int *p) {
            for (int i = 0; i < pointCount; i++) {
                p[i] = i;
            }

            permute(p, pointCount);
        }

        static void permute(int *p, int n) {
            for (int i = n - 1; i > 0; i--) {
                int target = randomInt(0, 1);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }

        static float trilinearInterpolation(vec3 c[2][2][2], float u, float v, float w) {
            float uu = u * u * (3 -2 * u);
            float vv = v * v * (3 -2 * v);
            float ww = w  *w * (3 - 2 * w);
            float accum = 0.0;

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        vec3 weightV(u - i, v - j, w - k);
                        accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weightV);
                    }
                }
            }
            return accum;
        }
};

#endif