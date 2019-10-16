#pragma once

#include "Color.h"
#include "Ray.h"

enum class MaterialType
{
   kReflectionAndRefraction,
   kReflection,
   kDiffuseAndGlossy,
};

class Element
{
public:
   Element() : material_type_(MaterialType::kDiffuseAndGlossy), albedo_(0.58f)
   {
   }

   virtual ~Element() = default;

   virtual bool Intersect(const Ray&, float&) = 0;
   virtual glm::vec3 GetSurfaceNormal(glm::vec3&) = 0;
   // getSurfaceProperties?

   MaterialType GetMaterialType() const { return material_type_; }
   glm::vec3 GetDiffuseColor() const { return diffuse_color_; } // Should Evaluate, const Vec2f & 
   float GetAlbedo() const { return albedo_; }

protected:
   // These should be packed up as Material class/struct
   MaterialType material_type_;
   glm::vec3 diffuse_color_;
   float albedo_;
   // specularExponent
};
