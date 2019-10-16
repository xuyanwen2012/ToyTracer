#include <fstream>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include <array>

#include "Ray.h"
#include "Color.h"
#include "Element.h"
#include "Sphere.h"

using ElementContainer = std::vector<std::unique_ptr<Element>>;

// Create a Ray from camera to pixel.
// 
Ray BuildPrimeRay(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
   const float fov_adjustment = glm::tan(glm::radians(90.0f) / 2.0f);
   const float aspect_ratio = width / static_cast<float>(height);

   float sensor_x = (static_cast<float>(x) + 0.5f) / static_cast<float>(width) * 2.0f - 1.0f;
   float sensor_y = 1.0f - (static_cast<float>(y) + 0.5f) / static_cast<float>(height) * 2.0f;

   sensor_x *= aspect_ratio * fov_adjustment;
   sensor_y *= fov_adjustment;

   return Ray(
      glm::vec3(),
      normalize(glm::vec3(sensor_x, sensor_y, -1.0))
   );
}

// The main tracing function. 
// 
// 
Color Trace(const Ray& ray, ElementContainer& elements, int depth)
{
   for (auto&& element : elements)
   {
      Element* target;
      float min_dist = INFINITY;

      if (element->Intersect(ray))
      {
         // Fix this
         return element->GetDiffuseColor();
      }
   }

   return Color::black();
}


int main()
{
   // Setup Scene
   const uint32_t kWidth = 800;
   const uint32_t kHeight = 600;

   ElementContainer elements;

   // first ball
   std::unique_ptr<Element> sphere_1_ptr = std::make_unique<Sphere>(
      glm::vec3{0.0f, 0.0f, -5.0f},
      1.0f,
      Color::red()
   );

   // second ball
   std::unique_ptr<Element> sphere_2_ptr = std::make_unique<Sphere>(
      glm::vec3{4.0f, 0.0f, -5.0f},
      1.0f,
      Color::green()
   );

   std::unique_ptr<Element> sphere_3_ptr = std::make_unique<Sphere>(
      glm::vec3{-4.0f, 0.0f, -5.0f},
      1.0f,
      Color::blue()
   );

   elements.push_back(std::move(sphere_1_ptr));
   elements.push_back(std::move(sphere_2_ptr));
   elements.push_back(std::move(sphere_3_ptr));


   // render image to buffer
   const auto frame_buffer = std::make_unique<glm::vec3[]>(kWidth * kHeight);

   for (uint32_t y = 0; y < kHeight; ++y)
   {
      for (uint32_t x = 0; x < kWidth; ++x)
      {
         Ray ray = BuildPrimeRay(kWidth, kHeight, x, y);

         // pixel should trace
         frame_buffer[x + y * kWidth] = Trace(ray, elements, 0).ToVec3();
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

   return EXIT_SUCCESS;
}
