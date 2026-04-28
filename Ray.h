//
// Created by ASUS on 2026/4/16.
//

#ifndef SOFTRAYTRACER_RAY_H
#define SOFTRAYTRACER_RAY_H
#include "Vector3f.hpp"

class Ray
{
    private:
    Vector3f _origin{0,0,0};
    Vector3f _direction{0,0,-1};
    public:
    Ray()=default;
    Ray(Vector3f origin, Vector3f direction):_origin(origin),_direction(direction){};
    [[nodiscard]]Vector3f at(float t) const { return _origin + t * _direction; }
    [[nodiscard]]const Vector3f& origin() const{return _origin;};
    [[nodiscard]]const Vector3f& direction() const{return _direction;}
};
#endif //SOFTRAYTRACER_RAY_H