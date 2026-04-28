//
// Created by ASUS on 2026/4/29.
//

#ifndef SOFTRAYTRACER_INTERVAL_H
#define SOFTRAYTRACER_INTERVAL_H

struct Interval
{
public:
    float min,max;
    Interval():min(+INFINITY),max(-INFINITY){}//默认为空
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

    static const Interval empty,universe;
};
const Interval Interval::empty{+INFINITY,-INFINITY};
const Interval Interval::universe{-INFINITY,+INFINITY};
#endif //SOFTRAYTRACER_INTERVAL_H