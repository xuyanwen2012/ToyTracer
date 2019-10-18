#pragma once

#include <glm/gtx/intersect.hpp>
#include <glm/gtx/normal.hpp>

#include "Element.h"

class Triangle : public Element
{
public:
   Triangle(Material material, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2) :
      Element(material),
      vertex_0_(v0),
      vertex_1_(v1),
      vertex_2_(v2)
   {
   }

   bool Intersect(const Ray& ray, float& dist) override
   {
      glm::vec2 bary_position;

      return intersectRayTriangle(
         ray.GetOrigin(), ray.GetDirection(),
         vertex_0_, vertex_1_, vertex_2_,
         bary_position, dist
      );
   }

   glm::vec3 GetSurfaceNormal(glm::vec3&) override
   {
      // cross(p1 - p2, p1 - p3), normalized
      return triangleNormal(vertex_0_, vertex_2_, vertex_1_);
   }

   glm::vec2 TextureCoords(glm::vec3& hit_point) override
   {
      // not implemented.
      return {};
   }

private:
   glm::vec3 vertex_0_;
   glm::vec3 vertex_1_;
   glm::vec3 vertex_2_;
};
