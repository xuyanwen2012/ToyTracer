#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <vector>
#include <array>

#include "Ray.h"
#include "Element.h"
#include "Sphere.h"
#include "Plane.h"
#include "Light.h"

using ElementContainer = std::vector<std::unique_ptr<Element>>;
using ElementPtr = std::unique_ptr<Element>;

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

   return {
      glm::vec3(),
      normalize(glm::vec3(sensor_x, sensor_y, -1.0))
   };
}

Color ShadeDiffuse(glm::vec3 hit_point, glm::vec3 surface_normal)
{
   return {};
}

// The main tracing function. 
// 
// 
Color Trace(
   const Ray& ray,
   ElementContainer& elements,
   std::unique_ptr<Light>& light_ptr, // tmp
   int depth
)
{
   Element* target = nullptr;
   float t_near = INFINITY;
   float dist = INFINITY; // intersect distance
   for (auto&& element : elements)
   {
      if (element->Intersect(ray, dist))
      {
         if (dist < t_near)
         {
            t_near = dist;
            target = element.get();
         }
      }
   }

   if (target != nullptr)
   {
      // Compute color
      auto hit_point = ray.GetOrigin() + ray.GetDirection() * dist;
      auto hit_normal = target->GetSurfaceNormal(hit_point);

      const auto direction_to_light = light_ptr->GetDirectionFrom(hit_point);

      // check shadow here
      auto shadow_ray = Ray{
         hit_point + hit_normal * 1e-13f, // bias
         direction_to_light
      };
      //auto shadow_intersection = Trace()
      // TODO: Fix
      bool in_shadow = false;
      {
         float _;
         for (auto&& element : elements)
         {
            if (element->Intersect(shadow_ray, _))
            {
               in_shadow = true;
               break;
            }
         }
      }

      const auto light_intensity = in_shadow ? 0.0f : light_ptr->GetIntensity();

      const float light_power = glm::max(0.0f, dot(hit_normal, direction_to_light)) * light_intensity;

      float light_reflected = target->GetAlbedo() / glm::pi<float>();

      //return hit_normal;

      const auto color = target->GetDiffuseColor() * light_ptr->GetColor() * light_power;
      // const auto color = hit_normal;

      return Color{
         glm::clamp(color.r, 0.0f, 1.0f),
         glm::clamp(color.g, 0.0f, 1.0f),
         glm::clamp(color.b, 0.0f, 1.0f)
      };
   }

   // temp return background color
   // TODO: use const
   return Color{0.11764705882f, 0.56470588235f, 1.0f};
}


int main()
{
   // Setup Scene
   const uint32_t kWidth = 1280;
   const uint32_t kHeight = 720;

   ElementContainer elements;

   // first ball
   ElementPtr sphere_1_ptr = std::make_unique<Sphere>(
      Colors::kRed,
      glm::vec3{0.0f, 0.0f, -5.0f},
      1.0f
   );

   // second ball
   ElementPtr sphere_2_ptr = std::make_unique<Sphere>(
      Colors::kGreen,
      glm::vec3{-3.0f, 1.0f, -6.0f},
      2.0f
   );

   // third ball
   ElementPtr sphere_3_ptr = std::make_unique<Sphere>(
      Colors::kBlue,
      glm::vec3{2.0f, 1.0f, -4.0f},
      1.5f
   );

   // Plane
   ElementPtr plane_ptr = std::make_unique<Plane>(
      Colors::kWhite,
      // origin
      glm::vec3{0.0f, 0.0f, -20.0f},
      // normal
      glm::vec3{0.0f, 1.0f, 0.0f}
      //glm::vec3{0.0f, -1.0f, 0.0f}
   );

   elements.push_back(std::move(sphere_1_ptr));
   elements.push_back(std::move(sphere_2_ptr));
   elements.push_back(std::move(sphere_3_ptr));
   elements.push_back(std::move(plane_ptr));

   // Adding light to the scene
   // TODO: Fix this temp code
   std::unique_ptr<Light> light_ptr = std::make_unique<DirectionLight>(
      Colors::kWhite,
      1.0f,
      glm::vec3{0.0f, 0.0f, -1.0f}
      //glm::vec3{ -1.0f, 0.0f, -1.0f }
   );

   // render image to buffer
   const auto frame_buffer = std::make_unique<glm::vec3[]>(kWidth * kHeight);

   for (uint64_t y = 0; y < kHeight; ++y)
   {
      for (uint64_t x = 0; x < kWidth; ++x)
      {
         Ray ray = BuildPrimeRay(kWidth, kHeight, x, y);

         // pixel should trace
         frame_buffer[x + y * kWidth] = Trace(ray, elements, light_ptr, 0);
      }
   }

   // saving the ppm 
   std::ofstream image("image.ppm");

   if (image.is_open())
   {
      image << "P3\n" << kWidth << " " << kHeight << " 255\n";

      for (uint64_t y = 0; y < kHeight; ++y)
      {
         for (uint64_t x = 0; x < kWidth; ++x)
         {
            const auto color = frame_buffer[x + y * kWidth];

            image << glm::clamp(static_cast<int>(color.r * 255), 0, 255) << " "
               << glm::clamp(static_cast<int>(color.g * 255), 0, 255) << " "
               << glm::clamp(static_cast<int>(color.b * 255), 0, 255) << " ";
         }
      }

      image.close();
   }

   return EXIT_SUCCESS;
}
