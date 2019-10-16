#pragma once
#include "Color.h"

class Light
{
public:
   Light(glm::vec3 color, float intensity) : color_{color}, intensity_{intensity}
   {
   }

   glm::vec3 GetColor() const { return color_; }
   float GetIntensity() const { return intensity_; }

private:
   glm::vec3 color_;
   float intensity_;
};

class DirectionLight : public Light
{
public:
   DirectionLight(glm::vec3 color, float intensity, glm::vec3 direction) :
      Light(color, intensity),
      direction_{direction}
   {
   }

   glm::vec3 GetDirection() const { return direction_; }

private:
   glm::vec3 direction_;
};
