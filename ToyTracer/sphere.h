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

   std::optional<float> Intersect(const Ray& ray) override
   {
      const auto l = origin_ - ray.GetOrigin();

      const auto adj = dot(l, ray.GetDirection());

      const auto d2 = dot(l, l) - adj * adj;

      const auto radius2 = radius_ * radius_;

      if (d2 > radius2)
      {
         return {};
      }

      const auto thc = glm::sqrt(radius2 - d2);
      auto t0 = adj - thc;
      auto t1 = adj + thc;

      if (t0 < 0.f && t1 < 0.f)
      {
         return {};
      }

      return t0 < t1 ? t0 : t1;
   }

   glm::vec3 GetNormal(const glm::vec3 pos) const override
   {
      return normalize(pos - origin_);
   }

private:
   glm::vec3 origin_;
   double radius_;
};
