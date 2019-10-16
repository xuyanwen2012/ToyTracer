#pragma once

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

   bool Intersect(const Ray& ray, float& t) override;

   glm::vec3 GetSurfaceNormal(glm::vec3& hit_point) override
   {
      return normalize(hit_point - origin_);
   }

private:
   glm::vec3 origin_;
   float radius_;
};
