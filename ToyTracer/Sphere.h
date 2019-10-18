#pragma once

#include <glm/gtx/intersect.hpp>

#include "Element.h"

class Sphere : public Element
{
public:
   Sphere(Material material, const glm::vec3& center, const float radius) :
      Element(material),
      origin_(center),
      radius_(radius)
   {
   }

   bool Intersect(const Ray& ray, float& dist) override
   {
      return intersectRaySphere(
         ray.GetOrigin(), ray.GetDirection(),
         origin_, radius_ * radius_,
         dist
      );
   }

   glm::vec3 GetSurfaceNormal(glm::vec3& hit_point) override
   {
      return (hit_point - origin_) / radius_; // from glm
   }

   glm::vec2 TextureCoords(glm::vec3& hit_point) override
   {
      const auto hit_vec = hit_point - origin_;

      return {
         (1.0 + atan2(hit_vec.z, hit_vec.x)) / glm::pi<float>() * 0.5f,
         acos(hit_vec.y / radius_) / glm::pi<float>()
      };
   }

private:
   glm::vec3 origin_;
   float radius_;
};
