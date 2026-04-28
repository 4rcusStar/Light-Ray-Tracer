//
// Created by ASUS on 2026/4/29.
//

#ifndef SOFTRAYTRACER_HITTABLELIST_H
#define SOFTRAYTRACER_HITTABLELIST_H
#include <memory>
#include <utility>
#include <vector>

#include "Hittable.h"

class HittableList: public Hittable
{
public:
    std::vector<std::shared_ptr<Hittable>> objects;

    HittableList()=default;
    HittableList(std::shared_ptr<Hittable> object){add(object);}

    void clear(){objects.clear();}

    void add(std::shared_ptr<Hittable> object){objects.push_back(object);}

    bool hit(const Ray& r, Interval tRange, HitRecord& record) const override
    {
        HitRecord tempRecord;
        bool hitAnything{false};
        auto closestSoFar{tRange.max};

        for (const auto& object : objects)
        {
            if (object->hit(r,Interval(tRange.min,closestSoFar),tempRecord))
            {
                hitAnything=true;
                closestSoFar=tempRecord.t;
                record=tempRecord;
            }
        }

        return hitAnything;
    }
};
#endif //SOFTRAYTRACER_HITTABLELIST_H