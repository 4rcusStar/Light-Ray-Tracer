//
// Created by ASUS on 2026/4/15.
//
#pragma once
#include <cmath>
#include "Interval.h"
#include "Vector3f.hpp"
using Color=Vector3f;
inline float linearToGamma(float linearComponent)
{
    if (linearComponent>0.f)
        return std::pow(linearComponent,1.f/2.2f);
    return 0;
}
static void writeColor(std::ostream& os, const Color& pixelColor)
{
    auto r{pixelColor.x};
    auto g{pixelColor.y};
    auto b{pixelColor.z};

    r=linearToGamma(r);
    g=linearToGamma(g);
    b=linearToGamma(b);

    static const Interval intensity(0,0.999999f);
    int rbyte{static_cast<int>(256 * intensity.clamp(r))};
    int gbyte{static_cast<int>(256 * intensity.clamp(g))};
    int bbyte{static_cast<int>(256 * intensity.clamp(b))};

    os << rbyte << ' '<<gbyte << ' '<<bbyte<<' ';
}