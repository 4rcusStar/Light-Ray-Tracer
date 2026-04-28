//
// Created by ASUS on 2026/4/29.
//
#ifndef SOFTRAYTRACER_CAMERA_H
#define SOFTRAYTRACER_CAMERA_H
#include <fstream>

#include "Color.h"
#include "Constants.h"
#include "Hittable.h"

class Camera
{
private:
    int imageHeight;
    Vector3f center{0};
    Vector3f pixelUpperLeft{0};
    Vector3f dU{0};
    Vector3f dV{0};
    float focalLength{1.f};
    float viewportWidth{-1};
    float viewportHeight{-1};
    void initialize()
    {
        imageHeight=static_cast<int>(imageWidth/aspectRatio)<1? 1:static_cast<int>(imageWidth/aspectRatio);
        viewportHeight = 2.f;
        viewportWidth = viewportHeight*static_cast<float>(imageWidth)/imageHeight;

        Vector3f viewportU{viewportWidth,0,0};
        Vector3f viewportV{0,-viewportHeight,0};
        dU= viewportU/imageWidth;
        dV= viewportV/imageHeight;

        pixelUpperLeft=center-Vector3f{0,0,focalLength}-viewportU/2-viewportV/2;
        pixelUpperLeft+={0.5f*dU+0.5f*dV};
    }
    Color rayColor(const Ray& r,const Hittable& world)
    {
        HitRecord record;
        if (world.hit(r,Interval(0,INFINITY),record))
        {
            return 0.5f*(record.normal+1);
        }
        Vector3f unitDirection{r.direction().normalized()};
        auto a = 0.5f*(unitDirection.y + 1.0f);
        return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    }
public:
    float aspectRatio{1.0f};
    int imageWidth{400};
    void render(const Hittable& world)
    {
        initialize();
        //渲染
        std::ofstream ofs("output.ppm");
        ofs<< "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

        for (int j{0};j<imageHeight;++j)
        {
            std::cout<<"Rendering line "<<j<<" "<<j*100./(imageHeight-1)<<"%"<<std::endl;
            for (int i{0};i<imageWidth;++i)
            {
                auto pixelCenter{pixelUpperLeft+i*dU+j*dV};
                auto rayDir{pixelCenter-center};//像素世界坐标减去相机世界坐标就是光线方向
                Ray ray(center,rayDir);
                Color pixelColor{rayColor(ray,world)};
                writeColor(ofs,pixelColor);
            }
            ofs<<std::endl;
        }
    }
};
#endif //SOFTRAYTRACER_CAMERA_H