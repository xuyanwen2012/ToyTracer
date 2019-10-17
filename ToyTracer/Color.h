#pragma once

#include <glm/glm.hpp>

using Color = glm::vec<3, float>;

namespace Colors
{
   constexpr Color kBlack{};
   constexpr Color kWhite{1.0f, 1.0f, 1.0f};
   constexpr Color kRed{1.0f, 0.0f, 0.0f};
   constexpr Color kGreen{0.0f, 1.0f, 0.0f};
   constexpr Color kBlue{0.0f, 0.0f, 1.0f};

   inline Color New(float r, float g, float b) { return {r, g, b}; }
}
