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
#include "Triangle.h"
#include "Light.h"
#include "Texture.h"

using ElementContainer = std::vector<std::unique_ptr<Element>>;
using ElementPtr = std::unique_ptr<Element>;
using LightContainer = std::vector<std::unique_ptr<Light>>;
using LightPtr = std::unique_ptr<Light>;

constexpr int kMaxRecursionDepth = 10;

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

Color GetBackgroundColor(const Ray& ray)
{
   const float t = (ray.GetDirection().y + 1.0f) * 0.5f;
   return (1.0f - t) * Colors::kWhite + t * Colors::New(0.5f, 0.7f, 1.0f);
}

Color ShadeDiffuse(
   ElementContainer& elements, LightContainer& lights,
   glm::vec3& hit_point, glm::vec3& hit_normal,
   Element* target
)
{
   // auto texture_coords = target->TextureCoords(hit_point);
   auto color = Colors::kBlack;

   // Check against all lights
   for (auto&& light : lights)
   {
      const auto direction_to_light = light->GetDirectionFrom(hit_point);

      // check shadow here, casting a second ray
      auto shadow_ray = Ray{
         hit_point + hit_normal * 1e-4f, // std::numeric_limits<float>::epsilon(), // bias
         direction_to_light
      };

      bool in_light = true;

      float shadow_dist = INFINITY;
      for (auto&& element : elements)
      {
         if (element->Intersect(shadow_ray, shadow_dist))
         {
            in_light = false;
            break;
         }
      }

      in_light |= shadow_dist > light->GetDistanceFrom(hit_point);

      const auto light_intensity = in_light ? light->GetIntensity(hit_point) : 0.0f;

      const float light_power = glm::max(0.0f, dot(hit_normal, direction_to_light)) * light_intensity;

      const float light_reflected = target->GetAlbedo() / glm::pi<float>();

      color += target->GetDiffuseColor() * light->GetColor() * light_power * light_reflected;
   }

   return color;
}

float Fresnel(const glm::vec3& incident, const glm::vec3& normal, float index)
{
   const float i_dot_n = dot(incident, normal);

   float eta_i = 1.0f;
   const float eta_t = index;

   if (i_dot_n > 0.0)
   {
      eta_i = eta_t;
      eta_i = 1.0;
   }

   const float sin_t = eta_i / eta_t * glm::sqrt(glm::max(1.0f - i_dot_n * i_dot_n, 0.0f));
   if (sin_t > 1.0)
   {
      return 1.0;
   }
   const float cos_t = glm::sqrt(glm::max(1.0f - sin_t * sin_t, 0.0f));
   const float cos_i = glm::abs(cos_t);
   const float r_s = ((eta_t * cos_i) - (eta_i * cos_t)) / ((eta_t * cos_i) + (eta_i * cos_t));
   const float r_p = ((eta_i * cos_i) - (eta_t * cos_t)) / ((eta_i * cos_i) + (eta_t * cos_t));

   return (r_s * r_s + r_p * r_p) / 2.0f;
}

// The main tracing function.
//
//
Color Trace(
   const Ray& ray,
   ElementContainer& elements,
   LightContainer& lights,
   int depth
)
{
   // Recursion check
   if (depth >= kMaxRecursionDepth)
   {
      return Colors::kRed;
   }

   // Check intersections, using a very *naive* brute force method.
   // Do not try this at home.
   auto list = std::vector<std::pair<float, Element*>>();

   for (auto&& element : elements)
   {
      float dist = INFINITY;
      if (element->Intersect(ray, dist))
      {
         list.emplace_back(dist, element.get());
      }
   }

   std::sort(list.begin(), list.end(), [](auto& left, auto& right)
   {
      return left.first < right.first;
   });

   auto target = list.empty() ? nullptr : list.front().second;
   auto dist = list.empty() ? INFINITY : list.front().first;

   // Get Color
   if (target != nullptr)
   {
      auto hit_point = ray.GetOrigin() + ray.GetDirection() * dist;
      auto hit_normal = target->GetSurfaceNormal(hit_point);

      auto final_color = Colors::kBlack;

      //return hit_normal;

      switch (target->GetMaterialType())
      {
      case MaterialType::kDiffuse:
         {
            final_color = ShadeDiffuse(elements, lights, hit_point, hit_normal, target);

            break;
         }
      case MaterialType::kReflective:
         {
            final_color = ShadeDiffuse(elements, lights, hit_point, hit_normal, target);

            const auto reflection_ray = Ray{
               hit_point + hit_normal * 1e-4f,
               reflect(ray.GetDirection(), hit_normal)
            };

            final_color *= 1 - target->GetReflectivity();
            final_color += Trace(reflection_ray, elements, lights, depth + 1) * target->GetReflectivity();

            break;
         }
      case MaterialType::kRefractive:
         {
            float kr = Fresnel(ray.GetDirection(), hit_normal, target->GetIndex());
            //float kr = 0.5f;

            auto surface_color = target->GetDiffuseColor();
            Color refraction_color = Colors::kWhite;

            if (kr < 1.0)
            {
               // Create transmission ray
               auto ref_n = hit_normal;
               auto eta_t = target->GetIndex();
               auto eta_i = 1.0f;
               auto i_dot_n = dot(ray.GetDirection(), hit_normal);
               if (i_dot_n < 0.0)
               {
                  //Outside the surface
                  i_dot_n = -i_dot_n;
               }
               else
               {
                  //Inside the surface; invert the normal and swap the indices of refraction
                  ref_n = -hit_normal;
                  eta_i = eta_t;
                  eta_t = 1.0f;
               }

               auto eta = eta_i / eta_t;
               float k = 1.0f - eta * eta * (1.0f - i_dot_n * i_dot_n);

               if (k >= 0.0)
               {
                  auto transmission_ray = Ray{
                     hit_point + ref_n * -1e-4f,
                     (ray.GetDirection() + i_dot_n * ref_n) * eta - ref_n * sqrt(k),
                  };

                  refraction_color = Trace(transmission_ray, elements, lights, depth + 1);
               }
            }

            // create a second ray
            const auto reflection_ray = Ray{
               hit_point + hit_normal * 1e-4f,
               reflect(ray.GetDirection(), hit_normal)
            };
            auto reflection_color = Trace(reflection_ray, elements, lights, depth + 1);

            final_color = reflection_color * kr + refraction_color * 1.0f - kr;
            final_color *= target->GetTransparency() * surface_color;

            break;
         }
      default: ;
      }

      // const auto color = hit_normal;
      return Color{
         glm::clamp(final_color.r, 0.0f, 1.0f),
         glm::clamp(final_color.g, 0.0f, 1.0f),
         glm::clamp(final_color.b, 0.0f, 1.0f)
      };
   }

   return GetBackgroundColor(ray);
}

void SetupScene(ElementContainer& elements, LightContainer& lights)
{
   // first ball
   ElementPtr sphere_1_ptr = std::make_unique<Sphere>(
      Material{
         MaterialType::kReflective,
         Colors::New(0.2f, 1.0f, 0.2f),
         0.18f,
         0.7f,
      },
      glm::vec3{0.0f, 0.0f, -5.0f},
      1.0f
   );

   // second ball
   ElementPtr sphere_2_ptr = std::make_unique<Sphere>(
      Material{
         MaterialType::kDiffuse,
         Colors::kRed, // texture
         0.58f,
      },
      glm::vec3{-3.0f, 1.0f, -6.0f},
      2.0f
   );

   // third ball
   ElementPtr sphere_3_ptr = std::make_unique<Sphere>(
      Material{
         MaterialType::kRefractive,
         Colors::kWhite,
         0.18f,
         0.5f,
         1.0f,
         1.5f,
      },
      glm::vec3{2.0f, 1.0f, -4.0f},
      1.5f
   );

   // Triangle
   ElementPtr triangle_1_ptr = std::make_unique<Triangle>(
      Material{
         MaterialType::kDiffuse,
         Colors::kRed,
         0.58f,
      },
      glm::vec3{1.0f - 14.0f, 0.0f, -8.0f},
      glm::vec3{3.0f - 14.0f, 4.0f, -6.0f},
      glm::vec3{5.0f - 14.0f, 0.0f, -8.0f}
   );

   // Plane
   ElementPtr plane_1_ptr = std::make_unique<Plane>(
      Material{
         MaterialType::kReflective,
         Colors::kWhite,
         0.18f,
         0.5f,
      },
      // origin
      glm::vec3{0.0f, -2.0f, -5.0f},
      // normal
      glm::vec3{0.0f, 1.0f, 0.0f}
   );

   ElementPtr plane_2_ptr = std::make_unique<Plane>(
      Material{
         MaterialType::kDiffuse,
         Colors::New(0.2f, 0.3f, 1.0f),
         0.38f,
      },
      // origin
      glm::vec3{0.0f, 0.0f, -20.0f},
      // normal
      glm::vec3{0.0f, 0.0f, 1.0f}
   );

   elements.push_back(std::move(sphere_1_ptr));
   elements.push_back(std::move(sphere_2_ptr));
   elements.push_back(std::move(sphere_3_ptr));
   //elements.push_back(std::move(triangle_1_ptr));
   elements.push_back(std::move(plane_1_ptr));
   elements.push_back(std::move(plane_2_ptr));

   // Adding light to the scene
   LightPtr light_s1_ptr = std::make_unique<SphericalLight>(
      Colors::New(0.3f, 0.8f, 0.3f),
      10000.0f,
      glm::vec3{-2.0f, 10.0f, -3.0f}
   );

   LightPtr light_s2_ptr = std::make_unique<SphericalLight>(
      Colors::New(0.8f, 0.3f, 0.3f),
      250.0f,
      glm::vec3{0.25f, 0.0f, -2.0f}
   );

   LightPtr light_d3_ptr = std::make_unique<DirectionLight>(
      Colors::kWhite,
      0.0f,
      glm::vec3{0.0f, 0.0f, -1.0f}
   );

   lights.push_back(std::move(light_s1_ptr));
   lights.push_back(std::move(light_s2_ptr));
   lights.push_back(std::move(light_d3_ptr));
}

int main()
{
   // Setup Scene
   const int32_t kWidth = 1280;
   const int32_t kHeight = 720;

   ElementContainer elements;
   LightContainer lights;

   SetupScene(elements, lights);

   // render image to buffer
   const auto frame_buffer = std::make_unique<glm::vec3[]>(kWidth * kHeight);

   for (auto y = 0; y < kHeight; ++y)
   {
      for (auto x = 0; x < kWidth; ++x)
      {
         Ray ray = BuildPrimeRay(kWidth, kHeight, x, y);

         // pixel should trace
         frame_buffer[x + y * kWidth] = Trace(ray, elements, lights, 0);
      }
   }

   // saving the ppm
   std::ofstream image("image.ppm");

   if (image.is_open())
   {
      image << "P3\n" << kWidth << " " << kHeight << " 255\n";

      for (auto y = 0; y < kHeight; ++y)
      {
         for (auto x = 0; x < kWidth; ++x)
         {
            const auto color = Colors::ToRGBA(frame_buffer[x + y * kWidth]);

            image << glm::clamp(color.r, 0, 255) << " "
               << glm::clamp(color.g, 0, 255) << " "
               << glm::clamp(color.b, 0, 255) << " ";
         }
      }

      image.close();
   }

   return EXIT_SUCCESS;
}
