#include <fstream>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include <array>

#include "Sphere.h"
#include "Ray.h"
#include "Color.h"

enum class MaterialType
{
   kReflectionAndRefraction,
   kReflection,
   kDiffuseAndGlossy,
};

using ElementContainer = std::vector<std::unique_ptr<Intersectable>>;


Ray BuildPrimeRay(uint32_t w, uint32_t h, uint32_t x, uint32_t y)
{
   const float fov_adjustment = glm::tan(glm::radians(90.0f) / 2.0f);
   const float aspect_ratio = w / static_cast<float>(h);

   float sensor_x = (static_cast<float>(x) + 0.5f) / static_cast<float>(w) * 2.0f - 1.0f;
   float sensor_y = 1.0f - (static_cast<float>(y) + 0.5f) / static_cast<float>(h) * 2.0f;

   sensor_x *= aspect_ratio * fov_adjustment;
   sensor_y *= fov_adjustment;

   return Ray(
      glm::vec3(),
      normalize(glm::vec3(sensor_x, sensor_y, -1.0))
   );
}

Color Trace(const Ray& ray)
{
   return Color::blue();
}


int main()
{
   ElementContainer elements;

   const uint32_t kWidth = 800;
   const uint32_t kHeight = 600;

   // render image to buffer
   //std::array<Color, kWidth * kHeight> frame_buffer {};
   /* Color* frame_buffer = new Color[kWidth * kHeight];*/

   auto frame_buffer = std::make_unique<glm::vec3[]>(kWidth * kHeight);

   //auto frame_buffer = std::make_unique<Color[]>(kWidth * kHeight);

   for (uint32_t y = 0; y < kHeight; ++y)
   {
      for (uint32_t x = 0; x < kWidth; ++x)
      {
         Ray ray = BuildPrimeRay(kWidth, kHeight, x, y);

         // pixel should trace
         frame_buffer[x + y * kWidth] = Trace(ray).ToVec3();
      }
   }

   // saving the ppm 
   std::ofstream image("image.ppm");

   if (image.is_open())
   {
      image << "P3\n" << kWidth << " " << kHeight << " 255\n";

      for (uint32_t y = 0; y < kHeight; ++y)
      {
         for (uint32_t x = 0; x < kWidth; ++x)
         {
            const auto color = frame_buffer[x + y * kWidth];

            image << color.r << " " << color.g << " " << color.b << " ";
         }
      }

      image.close();
   }

   //delete[] frame_buffer;

   return EXIT_SUCCESS;
}
