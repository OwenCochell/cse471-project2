#pragma once

#include "pch.h"

#include "graphics/GrImage.h"

void RippleTans(int x2, int y2, double x1, double y1);

BYTE interpolate(double x, double y, BYTE tl, BYTE tr, BYTE bl, BYTE br);

void applyRippleEffect(CGrImage& image, float time, float frequency, float amplitude);
