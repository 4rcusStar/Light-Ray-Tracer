#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"
#include "Material.h"


int main()
{
    Camera camera;
    camera.aspectRatio=16/9.f;
    camera.imageWidth=800;
    camera.samplesPerPixel=100;
    camera.maxDepth=50;
    //构建世界：
    HittableList world{};
    auto matGround {std::make_shared<Lambertian>(Color(0.8,0.8,0))};
    auto matCenter {std::make_shared<Lambertian>(Color(0.1,0.2,0.5))};
    auto matLeft{std::make_shared<Metal>(Color(0.8,0.8,0.8),0.3f)};
    auto matRight{std::make_shared<Metal>(Color(0.8,0.6,0.2),1.f)};

    world.add(std::make_shared<Sphere>(Vector3f{0,-100.5,-1}, 100,matGround));
    world.add(std::make_shared<Sphere>(Vector3f{0,0,-1.2}, .5,matCenter));
    world.add(std::make_shared<Sphere>(Vector3f{-1,0,-1}, .5,matLeft));
    world.add(std::make_shared<Sphere>(Vector3f{1,0,-1}, .5,matRight));
    camera.render(world);
}
