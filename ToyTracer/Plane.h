#pragma once

#include <glm/gtx/intersect.hpp>

#include "Element.h"
 
class Plane : public Element
{
public:
   Plane(Color color, glm::vec3 center, glm::vec3 normal) :
      Element(color),
      center_(center),
      normal_(normal)
   {
   }

   bool Intersect(const Ray& ray, float& t) override
   {
      return intersectRayPlane(
         ray.GetOrigin(), ray.GetDirection(),
         center_, normal_,
         t
      );
   }

   glm::vec3 GetSurfaceNormal(glm::vec3&) override { return normal_; }

private:
   glm::vec3 center_;
   glm::vec3 normal_;
};
