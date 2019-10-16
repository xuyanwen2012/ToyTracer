#include "Plane.h"

bool Plane::Intersect(const Ray& ray, float& t)
{
   const auto denom = dot(normal_, ray.GetDirection());

   if (denom > 1e-6)
   {
      const auto v = center_ - ray.GetOrigin();
      const auto distance = dot(v, normal_) / denom;
      if (distance >= 0.0)
      {
         t = distance;
         return true;
      }
   }

   return false;
}
