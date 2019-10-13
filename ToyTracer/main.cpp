#include <iostream>
#include <fstream>
#include <glm/glm.hpp>

#include "ray.h"
#include "sphere.h"

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

   auto aspect_ratio = width / height;

   const auto sensor_x = ((0.5f + static_cast<float>(x)) / width * 2.f - 1.f) * aspect_ratio;
   const auto sensor_y = 1.f - (0.5f + static_cast<float>(y)) / height * 2.f;

   return Ray{
      glm::vec3{},
      normalize(glm::vec3{sensor_x, sensor_y, -1.f})
   };
}


void render(const ViewBlock& block, Intersectable& object)
{
   std::ofstream image("image.ppm");

   if (image.is_open())
   {
      image << "P3\n" << block.width << " " << block.height << " 255\n";

      for (auto y = 0; y < block.height; ++y)
      {
         for (auto x = 0; x < block.width; ++x)
         {
            auto ray = CreatePrime(x, y);

            if (object.Intersect(ray))
            {
               auto v = ray.GetDirection();
               auto p = ray.GetOrigin() + v * 0.f;
               auto n = object.GetNormal(p);

               auto facing_ratio = dot(n, v);

               image << 102 << ' ' << 255 << ' ' << 102 << "\n";
            }
            else
            {
               image << 0 << ' ' << 0 << ' ' << 0 << "\n";
            }
         }
      }

      image.close();
   }
}

int main()
{
   using namespace glm;

   const size_t kWidth = 800;
   const size_t kHeight = 600;
   // fov

   const ViewBlock kBlock = {0, 0, kWidth, kHeight};

   auto sphere = Sphere{
      vec3{
         0.f,
         0.f,
         -5.f
      },
      1.f
   };

   render(kBlock, sphere);

   return EXIT_SUCCESS;
}
