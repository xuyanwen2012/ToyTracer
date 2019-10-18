#pragma once

#include "Color.h"

class Light
{
public:
   virtual ~Light() = default;

   Light(Color color, float intensity) : intensity_{intensity}, color_{color}
   {
   }

   virtual glm::vec3 GetDirectionFrom(const glm::vec3&) = 0;
   virtual float GetDistanceFrom(const glm::vec3&) = 0;

   virtual float GetIntensity(glm::vec3&) { return intensity_; }
   Color GetColor() const { return color_; }

protected:
   float intensity_;

private:
   Color color_;
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
   float GetDistanceFrom(const glm::vec3&) override { return INFINITY; }

private:
   glm::vec3 direction_;
};

class SphericalLight : public Light
{
public:
   SphericalLight(Color color, float intensity, glm::vec3 origin) :
      Light(color, intensity),
      origin_(origin)
   {
   }

   glm::vec3 GetDirectionFrom(const glm::vec3& point) override { return normalize(origin_ - point); }
   float GetDistanceFrom(const glm::vec3& point) override { return length(origin_ - point); }

   float GetIntensity(glm::vec3& point) override
   {
      const auto r2 = length(origin_ - point);
      return intensity_ / (4.0f * glm::pi<float>() * r2);
   }

private:
   glm::vec3 origin_;
};
