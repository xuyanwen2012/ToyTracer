#pragma once

// Faking a texture, since I don;t know how to load them
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.
// Do not try this at home.

#include "Color.h"
#include <vector>
#include <iostream>

// Fake texture,
class Texture
{
public:
   Texture(int width, int height) : width_(width), height_(height)
   {
      fake_data_ = new std::vector<Color>(width_ * height_);

      const int kN = 4;
      int block_len = width_ / kN;

      int fff = 0;
      for (int x = 0; x < width_; x += block_len)
      {
         for (int y = 0; y < height_; y += block_len)
         {
            const auto color = fff % 2 == 0 ? Colors::kWhite : Colors::kBlack;
            for (int i = 0; i < block_len; ++i)
            {
               for (int j = 0; j < block_len; ++j)
               {
                  fake_data_->operator[](x + y * width_) = color;
               }
            }
            ++fff;
         }
      }

      // Debug
      for (int x = 0; x < width_; ++x)
      {
         for (int y = 0; y < height_; ++y)
         {
            auto v = fake_data_->operator[](x + y * width_);

            std::cout << static_cast<int>(v.r);
         }

         std::cout << std::endl;
      }
   }

private:
   int width_;
   int height_;

   std::vector<Color>* fake_data_;
};
