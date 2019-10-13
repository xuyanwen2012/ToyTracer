#pragma once

#include "ray.h"

class Intersectable
{
public:
   Intersectable() = default;
   virtual ~Intersectable() = default;
   virtual bool Intersect(const Ray&) = 0;
   virtual glm::vec3 GetNormal(glm::vec3 pos) const = 0;
};
