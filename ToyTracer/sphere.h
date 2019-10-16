#pragma once

#include "Element.h"
#include "Color.h"

class Sphere : public Element
{
public:
   Sphere(const glm::vec3& center, const double radius, Color color) :
      origin_{ center },
      radius_{ radius }
   {
      diffuse_color_ = color;
   }

   bool Intersect(const Ray& ray) override;

private:
   glm::vec3 origin_;
   double radius_;
};
