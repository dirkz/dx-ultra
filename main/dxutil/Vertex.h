#pragma once

#include "stdafx.h"

namespace dxultra
{

struct Vertex
{
    XMFLOAT3 Position;
    XMFLOAT3 Color;

    Vertex(float x, float y, float z, XMVECTORF32 color) : Position{x, y, z}, Color{color}
    {
    }

    Vertex(double x, double y, double z, XMVECTORF32 color)
        : Position{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)},
          Color{color}
    {
    }
};

} // namespace dxultra
