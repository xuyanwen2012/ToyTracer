#pragma once

#include <optional>
#include "ray.h"

class Intersectable
{
public:
   Intersectable() = default;
   virtual ~Intersectable() = default;
   virtual std::optional<float> Intersect(const Ray&) = 0;
   virtual glm::vec3 GetNormal(glm::vec3 pos) const = 0;
};
