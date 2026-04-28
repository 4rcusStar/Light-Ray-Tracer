//
// Created by ASUS on 2026/4/15.
//
#pragma once
#include "Vector3f.hpp"
using Color=Vector3f;
static void writeColor(std::ostream& os, const Color& pixelColor)
{
    auto r{pixelColor.x};
    auto g{pixelColor.y};
    auto b{pixelColor.z};
    constexpr double COLOR_SCALE{255.9999};
    int rbyte{static_cast<int>(r * COLOR_SCALE)};
    int gbyte{static_cast<int>(g * COLOR_SCALE)};
    int bbyte{static_cast<int>(b * COLOR_SCALE)};

    os << rbyte << ' '<<gbyte << ' '<<bbyte<<' ';
}