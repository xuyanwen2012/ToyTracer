#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <vector>

#include "ray.h"
#include "sphere.h"
#include "plane.h"

struct Color
{
   uint8_t r;
   uint8_t g;
   uint8_t b;
   // alpha
};

constexpr Color Red = Color{255, 0, 0};
constexpr Color Black = Color{0, 0, 0};

struct ViewBlock
{
   int x;
   int y;
   int width;
   int height;
};

Ray CreatePrime(int x, int y)
{
   // TODO: Fix me
   auto width = 800.f;
   auto height = 600.f;

   assert(width > height);

   auto fov_adjustment = glm::tan(glm::radians(90.f) / 2.0f);
   auto aspect_ratio = width / height;

   const auto sensor_x = ((0.5f + static_cast<float>(x)) / width * 2.f - 1.f) * aspect_ratio * fov_adjustment;
   const auto sensor_y = (1.f - (0.5f + static_cast<float>(y)) / height * 2.f) * fov_adjustment;

   return Ray{
      glm::vec3{},
      normalize(glm::vec3{sensor_x, sensor_y, -1.f})
   };
}

std::unique_ptr<glm::vec3[]> RenderToBuffer(const ViewBlock& block, const std::vector<Intersectable*>& elements)
{
   const int w = block.width;
   const int h = block.height;

   auto frame_buffer = std::make_unique<glm::vec3[]>(w * h);

   for (auto y = 0; y < block.height; ++y)
   {
      for (auto x = 0; x < block.width; ++x)
      {
         // iterate through all objects
         Ray ray = CreatePrime(x, y);

         for (const auto object : elements)
         {
            if (auto t = object->Intersect(ray))
            {
               frame_buffer[x + y * w] = glm::vec3(255, 0, 0);
            }
            else
            {
               frame_buffer[x + y * w] = glm::vec3(0, 0, 0);
            }
         }
      }
   }

   return frame_buffer;
}

int main()
{
   using namespace glm;

   const size_t kWidth = 800;
   const size_t kHeight = 600;
   // fov

   const ViewBlock kBlock = {0, 0, kWidth, kHeight};

   // Constructing the scene
   auto sphere = Sphere{
      vec3{
         0.f,
         0.f,
         -5.f
      },
      1.f
   };

   auto plane = Plane{
      vec3{0.0, 0.0, -10.0}, vec3{0.0, 0.0, -1.0}
   };

   std::vector<Intersectable*> elements{
      &plane,
      &sphere,
   };

   // Trace
   const auto frame_buffer = RenderToBuffer(kBlock, elements);

   // Write to file
   std::ofstream image("image.ppm");

   if (image.is_open())
   {
      image << "P3\n" << kBlock.width << " " << kBlock.height << " 255\n";

      for (auto y = 0; y < kBlock.height; ++y)
      {
         for (auto x = 0; x < kBlock.width; ++x)
         {
            const auto color = frame_buffer[x + y * kBlock.width];

            image << color.r << " " << color.g << " " << color.b << " ";
         }
      }

      image.close();
   }

   return EXIT_SUCCESS;
}
