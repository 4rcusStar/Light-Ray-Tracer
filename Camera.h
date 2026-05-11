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
    int imageHeight{0};
    Vector3f center{0};
    Vector3f pixelUpperLeft{0};
    Vector3f dU{0};
    Vector3f dV{0};
    float viewportWidth{-1};
    float viewportHeight{-1};
    float pixelSamplesScale{0};//一个像素中每次采样得到颜色的系数=1/采样数

    Vector3f u{0},v{0},w{0};//CameraFrame Basis Vectors

    Vector3f defocusDiskU{1,0,0};
    Vector3f defocusDiskV{0,-1,0};

    void initialize()
    {
    // ==================== 1. 图像尺寸计算 ====================
    // 根据图像宽度和宽高比计算图像高度，至少为1像素
    imageHeight = static_cast<int>(imageWidth / aspectRatio) < 1
                  ? 1
                  : static_cast<int>(imageWidth / aspectRatio);

    // ==================== 2. 相机坐标系建立 ====================
    // 计算视口在世界空间中的尺寸
    // 注：此处依赖 u, v, w 已在外部初始化，但实际应在下方坐标系建立后使用
    auto fovTheta{MathUtils::degreesToRadians(verticalFov)};  // 垂直视场角转弧度
    auto h{std::tan(fovTheta / 2)};                           // 半视场角的正切值

    // 视口高度 = 2 * h * 焦距（基于相似三角形）
    viewportHeight = 2 * h * focusDistance;
    // 视口宽度 = 视口高度 * (图像宽度/图像高度)，保持像素比例
    viewportWidth = viewportHeight * static_cast<float>(imageWidth) / imageHeight;

    // ==================== 3. 相机坐标系（UVW基底） ====================
    center = lookFrom;                              // 相机位置（原点）
    w = (lookFrom - lookAt).normalized();           // 视线反方向（指向相机后方）
    u = (w.cross(vUp)).normalized();                // 相机右方向（垂直于w和世界向上方向）
    v = w.cross(u);                                 // 相机上方向（正交于w和u，构成右手系）

    // ==================== 4. 视口向量 ====================
    // 视口在世界空间中的水平和垂直向量
    Vector3f viewportU{viewportWidth * u};          // 视口宽度 * 相机右方向
    Vector3f viewportV{-viewportHeight * v};        // 视口高度 * 相机下方向（图像坐标y轴向下）

    // 每个像素在世界空间中的步长向量
    dU = viewportU / imageWidth;                    // 水平方向每像素的偏移
    dV = viewportV / imageHeight;                   // 垂直方向每像素的偏移

    // ==================== 5. 视口左上角像素位置 ====================
    // 视口平面位于 lookAt 处，但实际在相机前方 focalLength 处
    // 视口中心在相机前方 focalLength 处
    Vector3f viewportCenter = center - focusDistance * w;  // 沿视线方向前进 focalLength

    // 视口左上角 = 视口中心 - 视口宽度/2 - 视口高度/2
    pixelUpperLeft = viewportCenter - viewportU / 2 - viewportV / 2;

    // 偏移到第一个像素的中心（像素坐标对应像素中心）
     pixelUpperLeft += {0.5f * dU + 0.5f * dV};
     auto defocusRadius{focusDistance*std::tan(MathUtils::degreesToRadians(defocusAngle/2))};
     defocusDiskU = u*defocusRadius;
     defocusDiskV = v*defocusRadius;

    // ==================== 6. 采样缩放因子 ====================
    // 每个像素颜色的平均系数（多采样平均）
    pixelSamplesScale = 1.0f / samplesPerPixel;
}

    ///
    /// @return 生成光圈内的随机一点
    [[nodiscard]] Vector3f defocusDiskSample()const
    {
        auto p{Vector3f::randomInUnitDisk()};
        return center + p.x*defocusDiskU + p.y*defocusDiskV;
    }
    ///
    /// @param i 像素x坐标
    /// @param j 像素y坐标
    /// @return 在像素范围内随机取一点，与相机原点构成的光线
    Ray getRay(int i,int j)
    {
        auto offset{sampleSquare()};
        auto rayOrigin{defocusAngle<=0?center:defocusDiskSample()};
        auto pixelSample{pixelUpperLeft+ ((i+offset.x)*dU)+(j+offset.y)*dV};
        auto rayDir{pixelSample-rayOrigin};

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
    float verticalFov{90};
    Vector3f lookFrom{0,0,0};//相机位置
    Vector3f lookAt{0,0,-1};//相机看向向量
    Vector3f vUp{0,1,0};//相机顶
    float defocusAngle{0};
    float focusDistance{10};//焦距

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
