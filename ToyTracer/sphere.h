#pragma once

#include "Element.h"

class Sphere : public Element
{
public:
   Sphere(const glm::vec3& center, const double radius) :
      origin_{ center },
      radius_{ radius }
   {
   }

   bool Intersect(const Ray& ray) override;

private:
   glm::vec3 origin_;
   double radius_;
};
