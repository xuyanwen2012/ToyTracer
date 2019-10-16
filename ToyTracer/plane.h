#pragma once

#include "Intersectable.h"

class Plane : public Intersectable
{
public:
   Plane(const glm::vec3& origin, const glm::vec3& normal) :
      origin_(origin),
      normal_(normal)
   {
   }

   std::optional<float> Intersect(const Ray& ray) override
   {
      const auto denom = dot(normal_, ray.GetDirection());
      if (denom > 1e-6)
      {
         const auto v = origin_ - ray.GetOrigin();
         const auto distance = dot(v, normal_) / denom;
         if (distance >= 0.0)
         {
            return distance;
         }
      }
      return {};
   }

   glm::vec3 GetNormal(glm::vec3) const override { return -normal_; }
private:
   glm::vec3 origin_;
   glm::vec3 normal_;
};
