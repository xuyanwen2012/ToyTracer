#include <fstream>
#include <glm/glm.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <vector>
#include <array>

#include "Ray.h"
// #include "Color.h"
#include "Element.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"

using ElementContainer = std::vector<std::unique_ptr<Element>>;
// using Colour = glm::vec<3, uint8_t>;

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
glm::vec3 Trace(
   const Ray& ray,
   ElementContainer& elements,
   std::unique_ptr<DirectionLight>& light_ptr, // tmp
   int depth
)
{
   Element* target = nullptr;
   float tnear = INFINITY;
   float t = INFINITY; // intersect distance

   for (auto&& element : elements)
   {
      if (element->Intersect(ray, t))
      {
         if (t < tnear)
         {
            tnear = t;
            target = element.get();
         }
      }
   }

   if (target != nullptr)
   {
      // Compute Illumination
      auto hit_point = ray.GetOrigin() + ray.GetDirection() * t;
      auto surface_normal = target->GetSurfaceNormal(hit_point);
      auto direction_to_light = -light_ptr->GetDirection();

      float light_power = dot(surface_normal, direction_to_light);
      light_power = std::max(0.0f, light_power) * light_ptr->GetIntensity();

      float light_reflected = target->GetAlbedo() / glm::pi<float>();

      // need to implemnt operator for color

      // * light_ptr->GetColor() * light_power * light_reflected

      // * light_reflected
      //  * light_power

      //return surface_normal;

      // Clamp?
      const auto color = target->GetDiffuseColor() * light_ptr->GetColor() * light_power;
      // *light_reflected;

      return glm::vec3{
         glm::clamp(color.r, 0.0f, 1.0f),
         glm::clamp(color.g, 0.0f, 1.0f),
         glm::clamp(color.b, 0.0f, 1.0f)
      };
   }

   return glm::vec3{};
}


int main()
{
   // Setup Scene
   const uint32_t kWidth = 800;
   const uint32_t kHeight = 600;

   ElementContainer elements;

   // first ball
   std::unique_ptr<Element> sphere_1_ptr = std::make_unique<Sphere>(
      glm::vec3{-2.0f, 0.0f, -3.0f},
      1.0f,
      glm::vec3{1.0f, 0.0f, 0.0f}
   );

   // second ball
   std::unique_ptr<Element> sphere_2_ptr = std::make_unique<Sphere>(
      glm::vec3{0.0f, 0.0f, -5.0f},
      1.0f,
      glm::vec3{0.0f, 1.0f, 0.0f}
   );

   // third ball
   std::unique_ptr<Element> sphere_3_ptr = std::make_unique<Sphere>(
      glm::vec3{2.0f, 0.0f, -7.0f},
      1.0f,
      glm::vec3{0.0f, 0.0f, 1.0f}
   );

   // Plane
   std::unique_ptr<Element> plane_ptr = std::make_unique<Plane>(
      glm::vec3{0.0f, -1.0f, 0.0f},
      glm::vec3{0.0f, -1.0f, 0.0f},
      glm::vec3{0.5f, 0.5f, 0.5f}
   );

   elements.push_back(std::move(plane_ptr));
   elements.push_back(std::move(sphere_1_ptr));
   elements.push_back(std::move(sphere_2_ptr));
   elements.push_back(std::move(sphere_3_ptr));

   // Adding light to the scene
   // TODO: Fix this temp code
   auto light_ptr = std::make_unique<DirectionLight>(
      glm::vec3{1.0f, 1.0f, 1.0f},
      // Color::white(),
      1.0f,
      glm::vec3{0.0f, 0.0f, -1.0f}
   );

   // render image to buffer
   const auto frame_buffer = std::make_unique<glm::vec3[]>(kWidth * kHeight);

   for (uint32_t y = 0; y < kHeight; ++y)
   {
      for (uint32_t x = 0; x < kWidth; ++x)
      {
         Ray ray = BuildPrimeRay(kWidth, kHeight, x, y);

         // pixel should trace
         frame_buffer[x + y * kWidth] = Trace(ray, elements, light_ptr, 0); // .ToVec3();
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

            image << std::clamp<int>(color.r * 255, 0, 255) << " "
               << std::clamp<int>(color.g * 255, 0, 255) << " "
               << std::clamp<int>(color.b * 255, 0, 255) << " ";
         }
      }

      image.close();
   }

   return EXIT_SUCCESS;
}
