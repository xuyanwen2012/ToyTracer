#pragma once

#include "Ray.h"

class Element
{
public:
   virtual ~Element() = default;
   virtual bool Intersect(const Ray&) = 0;
};

