#pragma once

#include "Color.h"
#include "Ray.h"

enum class MaterialType
{
   kDiffuse,
   kReflective,
   kRefractive,
};

class Element
{
public:
   Element(Color color) :
      material_type_(MaterialType::kDiffuse),
      diffuse_color_(color),
      albedo_(0.58f)
   {
   }

   virtual ~Element() = default;

   virtual bool Intersect(const Ray&, float& t) = 0;
   virtual glm::vec3 GetSurfaceNormal(glm::vec3&) = 0;

   MaterialType GetMaterialType() const { return material_type_; }
   Color GetDiffuseColor() const { return diffuse_color_; } // Should Evaluate, const Vec2f & 
   float GetAlbedo() const { return albedo_; }

protected:
   // These should be packed up as Material class/struct
   MaterialType material_type_;
   Color diffuse_color_;
   float albedo_;
};
