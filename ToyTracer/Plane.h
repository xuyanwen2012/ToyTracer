#pragma once

#include "Element.h"

class Plane : public Element
{
public:
   Plane(glm::vec3 center, glm::vec3 normal, Color color) :
      center_{center},
      normal_{normal}
   {
      diffuse_color_ = color;
   }

   bool Intersect(const Ray& ray, float& t) override;

   glm::vec3 GetSurfaceNormal(glm::vec3&) override { return -normal_; }

private:
   glm::vec3 center_;
   glm::vec3 normal_;
};