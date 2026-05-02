#include "Camera.h"
#include "Hittable.h"
#include "HittableList.h"
#include "Sphere.h"



int main()
{
    Camera camera;
    camera.aspectRatio=16/9.f;
    camera.imageWidth=400;
    camera.samplesPerPixel=100;
    camera.maxDepth=50;
    //构建世界：
    HittableList world{};
    world.add(std::make_shared<Sphere>(Vector3f{0,0,-1},.5f));
    world.add(std::make_shared<Sphere>(Vector3f{0,-100.5f,-1},100.f));
    //world.add(std::make_shared<Sphere>(Vector3f{0,3,-4},.2f));
    camera.render(world);
}
