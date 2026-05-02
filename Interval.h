//
// Created by ASUS on 2026/4/29.
//

#ifndef SOFTRAYTRACER_INTERVAL_H
#define SOFTRAYTRACER_INTERVAL_H
#include "Constants.h"

struct Interval
{
public:
    float min,max;
    Interval():min(+infinity),max(-infinity){}//默认为空
    Interval(float min,float max):min(min),max(max){}
    [[nodiscard]] float size() const
    {
        return max-min;
    }
    [[nodiscard]] bool contains(float x) const
    {
        return min<=x && x<=max;
    }
    [[nodiscard]] bool surrounds(float x) const
    {
        return min<x && x<max;
    }
    [[nodiscard]] float clamp(float x)const
    {
        if (x<min) return min;
        if (x>max) return max;
        return x;
    }
    static const Interval empty,universe;
};
const Interval Interval::empty{+infinity,-infinity};
const Interval Interval::universe{-infinity,+infinity};
#endif //SOFTRAYTRACER_INTERVAL_H