#pragma once

#include <glm/glm.hpp>
#include <cstdint>

struct Color
{
   static constexpr Color red() { return Color(255, 0, 0); }
   static constexpr Color green() { return Color(0, 255, 0); }
   static constexpr Color blue() { return Color(0, 0, 255); }
   static constexpr Color black() { return Color(0, 0, 0); }
   static constexpr Color white() { return Color(255, 255, 255); }

   //Color() = default; // should I fix this?
   constexpr Color() : r(0), g(0), b(0), a(255)
   {
   }

   constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
      : r(r), g(g), b(b), a(a)
   {
   }

   glm::vec3 ToVec3() const { return {r, g, b}; }

   uint8_t r;
   uint8_t g;
   uint8_t b;
   uint8_t a;
};
