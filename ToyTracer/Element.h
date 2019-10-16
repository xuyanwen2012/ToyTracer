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
   Element() : material_type_(MaterialType::kDiffuseAndGlossy)
   {
   }

   virtual ~Element() = default;
   virtual bool Intersect(const Ray&) = 0;
   // getSurfaceProperties?

   MaterialType GetMaterialType() const { return material_type_; }
   // Color GetDiffuseColor() const { return diffuse_color_; } // Should Evaluate, const Vec2f & 

protected:
   MaterialType material_type_;
   Color diffuse_color_;
   // specularExponent
};
