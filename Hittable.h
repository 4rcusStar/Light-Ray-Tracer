//
// Created by ASUS on 2026/4/28.
//

#ifndef SOFTRAYTRACER_HITTABLE_H
#define SOFTRAYTRACER_HITTABLE_H
#include <memory>

#include "Interval.h"
#include "Ray.h"
#include "Vector3f.hpp"
class Material;
struct  HitRecord
{
    Vector3f p{0};
    float t{0};
    Vector3f normal{0};///法线总是朝光线相反的方向
    bool isFrontFace{false};///是否指向面外侧
    std::shared_ptr<Material> mat{nullptr};
public:
    ///
    /// @param r 光线
    /// @param outwardNormal **单位向量** 外法线
    void setFaceNormal(const Ray& r, const Vector3f& outwardNormal)
    {
        isFrontFace = outwardNormal.dot(r.direction()) < 0;
        normal = isFrontFace ? outwardNormal : -1*outwardNormal;
    };
};
class Hittable
{
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& r,Interval tRange,HitRecord& hitRecord) const = 0;
};
#endif //SOFTRAYTRACER_HITTABLE_H