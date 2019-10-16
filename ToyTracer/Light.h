#pragma once
#include "Color.h"

class Light
{
public:
   Light(Color color, float intensity) : color_{color}, intensity_{intensity}
   {
   }

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
      direction_{direction}
   {
   }

   glm::vec3 GetDirection() const { return direction_; }

private:
   glm::vec3 direction_;
};
