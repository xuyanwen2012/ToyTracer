#include "Sphere.h"

bool Sphere::Intersect(const Ray& ray)
{
   const auto l = origin_ - ray.GetOrigin();
   const auto adj = dot(l, ray.GetDirection());
   const auto d2 = dot(l, l) - adj * adj;
   const auto radius2 = radius_ * radius_;

   if (d2 > radius2)
   {
      return false;
   }

   const auto thc = glm::sqrt(radius2 - d2);
   auto t0 = adj - thc;
   auto t1 = adj + thc;

   if (t0 < 0.f && t1 < 0.f)
   {
      return false;
   }

   return true; // t0 < t1 ? t0 : t1;
}
