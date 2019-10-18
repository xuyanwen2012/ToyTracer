#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
   Ray(const glm::vec3& from, const glm::vec3& to) :
      direction_(),
      origin_(from)
   {
      // Have to normalize direction
      direction_ = normalize(to);
   }

   const glm::vec3& GetOrigin() const { return origin_; }
   const glm::vec3& GetDirection() const { return direction_; }

private:
   glm::vec3 direction_;
   glm::vec3 origin_;
};
