#pragma once

#include "Color.h"
#include "Ray.h"

enum class MaterialType
{
   kDiffuse,
   kReflective,
   kRefractive,
};

struct Material
{
   MaterialType type = MaterialType::kDiffuse;
   Color diffuse_color = Colors::kWhite;
   float albedo = 0.58f;
   float reflectivity = 0.5f;
   float transparency = 1.0f;
};

class Element
{
public:
   Element(Material material) :
      material_(material)
   {
   }

   virtual ~Element() = default;

   virtual bool Intersect(const Ray&, float& t) = 0;
   virtual glm::vec3 GetSurfaceNormal(glm::vec3&) = 0;

   MaterialType GetMaterialType() const { return material_.type; }
   Color GetDiffuseColor() const { return material_.diffuse_color; } // Should Evaluate, const Vec2f & 
   float GetAlbedo() const { return material_.albedo; }
   float GetReflectivity() const { return material_.reflectivity; }
   float GetTransparency() const { return material_.transparency; }

protected:
   Material material_;
};
