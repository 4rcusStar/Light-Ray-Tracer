//
// Created by ASUS on 2026/4/29.
//
#ifndef SOFTRAYTRACER_CAMERA_H
#define SOFTRAYTRACER_CAMERA_H
#include <fstream>

#include "Color.h"
#include "Constants.h"
#include "Hittable.h"
#include "Material.h"
#include "MathUtils.h"

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
    float pixelSamplesScale{0};//一个像素中每次采样得到颜色的系数=1/采样数
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

        pixelSamplesScale=1.f/samplesPerPixel;
    }

    ///
    /// @param i 像素x坐标
    /// @param j 像素y坐标
    /// @return 在像素范围内随机取一点，与相机原点构成的光线
    Ray getRay(int i,int j)
    {
        auto offset{sampleSquare()};
        auto rayOrigin{center};
        auto pixelSample{pixelUpperLeft+ ((i+offset.x)*dU)+(j+offset.y)*dV};
        auto rayDir{pixelSample-center};

        return Ray{rayOrigin,rayDir};
    }

    [[nodiscard]] static Vector3f sampleSquare()
    {
        return Vector3f{MathUtils::randomFloat()-.5f,MathUtils::randomFloat()-.5f,0};
    }

    Color rayColor(const Ray& r,int depth,const Hittable& world)
    {
        if (depth<=0)
            return Color{0,0,0};
        HitRecord record;
        if (world.hit(r,Interval(.0001f,infinity),record))
        {
            Ray scattered;
            Color attenuation{1.f};
            if (record.mat->scatter(r,record,attenuation,scattered))
            {
                return attenuation*rayColor(scattered,depth-1,world);
            }
            return Color{0,0,0};
        }
        Vector3f unitDirection{r.direction().normalized()};
        auto a = 0.5f*(unitDirection.y + 1.0f);
        return (1.0f-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0);
    }

public:
    float aspectRatio{1.0f};
    int imageWidth{400};
    int samplesPerPixel{10};
    int maxDepth{10};
    void render(const Hittable& world)
    {
        initialize();
        //渲染
        std::ofstream ofs("output.ppm");
        ofs<< "P3\n" << imageWidth << " " << imageHeight << "\n255\n";

        const int lineBreak = 10;
        for (int j{0};j<imageHeight;++j)
        {
            std::cout<<"Rendering line "<<j<<" "<<j*100./(imageHeight-1)<<"%"<<std::endl;
            for (int i{0};i<imageWidth;++i)
            {
                Color pixelColor{0};
                for (int sample{0};sample<samplesPerPixel;++sample)
                {
                    Ray ray{getRay(i,j)};
                    pixelColor+=rayColor(ray,maxDepth,world);
                }
                writeColor(ofs,pixelSamplesScale*pixelColor);
                if ((i + 1) % lineBreak == 0)
                    ofs << '\n';
            }
            ofs<<std::endl;
        }
    }
};
#endif //SOFTRAYTRACER_CAMERA_H