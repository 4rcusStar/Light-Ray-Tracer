//
// Created by ASUS on 2026/5/3.
//

#ifndef SOFTRAYTRACER_MATERIAL_H
#define SOFTRAYTRACER_MATERIAL_H
#include "Color.h"
#include "Hittable.h"
#include "Ray.h"

class Material
{
public:
    virtual ~Material() = default;

    virtual bool scatter(
        const Ray& ray,const HitRecord& hitRecord,Color& attenuation,Ray& scattered
        )const
    {
        return false;
    }
};
class Lambertian : public Material
{
private:
    Color _albedo;
public:
    explicit Lambertian(const Color& albedo):_albedo(albedo){};

    bool scatter(const Ray& ray, const HitRecord& hitRecord,Color& attenuation,Ray& scattered) const override
    {
        auto scatterDirection {hitRecord.normal+Vector3f::randomUnitVector()};
        if (scatterDirection.isZero())
            scatterDirection = hitRecord.normal;
        scattered = Ray(hitRecord.p,scatterDirection);
        attenuation = _albedo;
        return true;
    }
};
class Metal : public Material
{
private:
    Color _albedo;
    float _fuzz{0};
public:
    explicit Metal(const Color& albedo,const float fuzz):_albedo(albedo),_fuzz(fuzz){}
    explicit Metal(const Color& albedo):_albedo(albedo){};
    bool scatter(const Ray& ray, const HitRecord& hitRecord,Color& attenuation,Ray& scattered) const override
    {
        Vector3f reflected{Vector3f::reflect(ray.direction(),hitRecord.normal).normalized()+_fuzz*Vector3f::randomUnitVector()};
        scattered = Ray(hitRecord.p,reflected);
        attenuation = _albedo;
        return scattered.direction().dot(hitRecord.normal)>0;
    }
};
#endif //SOFTRAYTRACER_MATERIAL_H