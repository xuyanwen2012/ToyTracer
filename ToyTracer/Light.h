#pragma once
#include "Color.h"

class Light
{
public:
   virtual ~Light() = default;

   Light(Color color, float intensity) : color_{color}, intensity_{intensity}
   {
   }

   virtual glm::vec3 GetDirectionFrom(const glm::vec3& point) = 0;

   Color GetColor() const { return color_; }
   float GetIntensity() const { return intensity_; }

private:
   Color color_;
   float intensity_;
};

class DirectionLight : public Light
{
public:
   DirectionLight(Color color, float intensity, glm::vec3 direction) :
      Light(color, intensity),
      direction_()
   {
      direction_ = normalize(direction);
   }

   glm::vec3 GetDirectionFrom(const glm::vec3&) override { return -direction_; }

private:
   glm::vec3 direction_;
};
