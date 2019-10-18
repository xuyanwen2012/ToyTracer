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
   float index = 1.5f;
};

uint32_t Wrap(float val, uint32_t bound)
{
   const auto signed_bound = static_cast<int32_t>(bound);
   const float float_coord = val * bound;

   const auto wrapped_coord = static_cast<int32_t>(float_coord) % signed_bound;
   if (wrapped_coord < 0)
   {
      return static_cast<uint32_t>(wrapped_coord + signed_bound);
   }
   return static_cast<uint32_t>(wrapped_coord);
}

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
   virtual glm::vec2 TextureCoords(glm::vec3&) = 0;

   Color GetColoration(const glm::vec2& coords) const
   {
      auto tex_x = Wrap(coords.x, 2); // texture.width()
      auto tex_y = Wrap(coords.y, 2);

      // texture.get_pixel(tex_x, tex_y)
      return {};
      //Color::from_rgba(texture.texture.get_pixel(tex_x, tex_y));
   }

   MaterialType GetMaterialType() const { return material_.type; }
   Color GetDiffuseColor() const { return material_.diffuse_color; } // Should Evaluate, const Vec2f &

   float GetAlbedo() const { return material_.albedo; }
   float GetReflectivity() const { return material_.reflectivity; }
   float GetTransparency() const { return material_.transparency; }
   float GetIndex() const { return material_.index; }

protected:
   Material material_;
   // texture
};
