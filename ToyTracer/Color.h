//#pragma once
//
//#include <glm/glm.hpp>
//#include <cstdint>
//#include <algorithm>
//
//struct Color
//{
//   static constexpr Color red() { return Color(255, 0, 0); }
//   static constexpr Color green() { return Color(0, 255, 0); }
//   static constexpr Color blue() { return Color(0, 0, 255); }
//   static constexpr Color black() { return Color(0, 0, 0); }
//   static constexpr Color white() { return Color(255, 255, 255); }
//
//   //Color() = default; // should I fix this?
//   constexpr Color() : r(0), g(0), b(0), a(255)
//   {
//   }
//
//   constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
//      : r(r), g(g), b(b), a(a)
//   {
//   }
//
//   // TODO: need to optimize
//   Color operator*(const Color& other) const
//   {
//      return {
//         static_cast<uint8_t>(r * other.r),
//         static_cast<uint8_t>(g * other.g),
//         static_cast<uint8_t>(b * other.b),
//         static_cast<uint8_t>(a * other.a),
//      };
//   }
//
//   Color operator*(float scalar) const
//   {
//      return {
//         static_cast<uint8_t>(r * scalar),
//         static_cast<uint8_t>(g * scalar),
//         static_cast<uint8_t>(b * scalar),
//         static_cast<uint8_t>(a * scalar),
//      };
//   }
//
//   Color Clamp()
//   {
//      // https://codereview.stackexchange.com/questions/6502/fastest-way-to-clamp-an-integer-to-the-range-0-255
//
//      return {
//         r > UINT8_MAX ? UINT8_MAX : r,
//         g > UINT8_MAX ? UINT8_MAX : g,
//         b > UINT8_MAX ? UINT8_MAX : b,
//         a > UINT8_MAX ? UINT8_MAX : a
//      };
//   }
//
//   glm::vec3 ToVec3() const { return {r, g, b}; }
//
//   uint8_t r;
//   uint8_t g;
//   uint8_t b;
//   uint8_t a;
//};
