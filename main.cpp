#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"


int main()
{
    HittableList world;

    auto groundMat{std::make_shared<Lambertian>(Color{.5,})};
    world.add(std::make_shared<Sphere>(Vector3f{0,-1000,0},1000,groundMat));

    for (int a{-11};a<11;++a)
    {
        for (int b{-11};b<11;++b)
        {
            auto chooseMat{MathUtils::randomFloat()};
            Vector3f center(a+.9*MathUtils::randomFloat(),.2,b+.9*MathUtils::randomFloat());
            if ((center-Vector3f{4,.2,0}).length()>.9)
            {
                std::shared_ptr<Material> sphereMat;
                if (chooseMat<.8)
                {
                    auto albedo{Color::random()*Color::random()};
                    sphereMat = std::make_shared<Lambertian>(albedo);
                    world.add(std::make_shared<Sphere>(center,.2,sphereMat));
                }else if (chooseMat<.95)
                {
                    auto albedo{Color::random(.5,1)};
                    auto fuzz{MathUtils::randomFloat(0,.5f)};
                    sphereMat = std::make_shared<Metal>(albedo,fuzz);
                    world.add(std::make_shared<Sphere>(center,.2,sphereMat));
                }
                else
                {
                    sphereMat = std::make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center,.2,sphereMat));
                }
            }
        }
    }
    auto mat1{std::make_shared<Dielectric>(1.5)};
    world.add(std::make_shared<Sphere>(Vector3f{0,1,0},1.,mat1));

    auto mat2{std::make_shared<Lambertian>(Vector3f{.4,.2,.1})};
    world.add(std::make_shared<Sphere>(Vector3f{-4,1,0},1.,mat2));

    auto mat3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<Sphere>(Vector3f(4, 1, 0), 1.0, mat3));

    Camera cam;
    cam.aspectRatio=16.f/9.f;
    cam.imageWidth=1920;
    cam.samplesPerPixel=500;
    cam.maxDepth =50;

    cam.verticalFov = 20;
    cam.lookFrom = Vector3f{13,2,3};
    cam.vUp = Vector3f{0,1,0};

    cam.defocusAngle = .6;
    cam.focusDistance =10.f;

    cam.render(world);
}

