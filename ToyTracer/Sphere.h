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

private:
   glm::vec3 origin_;
   float radius_;
};
