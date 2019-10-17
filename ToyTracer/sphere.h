#pragma once

#include <glm/gtx/intersect.hpp>

#include "Element.h"
#include "Color.h"

class Sphere : public Element
{
public:
   Sphere(Color color, const glm::vec3& center, const float radius) :
      Element(color),
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
