#pragma once

#include <glm/gtx/scalar_multiplication.hpp>
#include "intersectable .h"

class Sphere : public Intersectable
{
public:
   Sphere(const glm::vec3& center, const double radius) :
      origin_{center},
      radius_{radius}
   {
   }

   bool Intersect(const Ray& ray) override
   {
      const auto l = origin_ - ray.GetOrigin();

      const auto adj2 = dot(l, ray.GetDirection());

      const auto d2 = dot(l, l) - adj2 * adj2;

      return d2 < radius_ * radius_;
   }

   glm::vec3 GetNormal(const glm::vec3 pos) const override
   {
      return (pos - origin_) * (-1.f / radius_);
   }

private:
   glm::vec3 origin_;
   double radius_;
};
