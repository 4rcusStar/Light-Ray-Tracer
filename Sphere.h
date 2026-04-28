//
// Created by ASUS on 2026/4/28.
//

#ifndef SOFTRAYTRACER_SPHERE_H
#define SOFTRAYTRACER_SPHERE_H
#include <cmath>

#include "Hittable.h"

class Sphere:public Hittable
{
private:
    Vector3f center;
    float radius;
public:
    Sphere(Vector3f center, float radius):center(center),radius(radius){};

    /// hit检测
    /// @param r 光线
    /// @param tRange t范围
    /// @param hitRecord hitRecord结构体记录击中表面信息
    /// @return 是否hit
    bool hit(const Ray &r,Interval tRange, HitRecord &hitRecord) const override
    {
        Vector3f originToCenter{center-r.origin()};
        float a{r.direction().dot(r.direction())};
        float h{(r.direction()).dot(originToCenter)};
        float c{originToCenter.dot(originToCenter)-radius*radius};
        float discriminant{h*h-a*c};
        if (discriminant < 0)
            return false;
        float sqrtD{std::sqrt(discriminant)};
        float root {(h-sqrtD)/a};

        //若求交公式解得t在范围外，则返回false
        if (!tRange.surrounds(root))
        {
            root = (h+sqrtD)/a;
            if (!tRange.surrounds(root))
                return false;
        }

        hitRecord.t = root;
        hitRecord.p = r.at(hitRecord.t);
        auto outwardNormal = (hitRecord.p - center)/radius;
        hitRecord.setFaceNormal(r, outwardNormal);

        return true;
    }



};
#endif //SOFTRAYTRACER_SPHERE_H