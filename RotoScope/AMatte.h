#pragma once

#include "pch.h"

#include <vector>
#include <memory>
#include <utility>

#include "graphics/GrImage.h"

using am_type = std::unique_ptr<std::vector<std::vector<double>>>;

/**
* Creates an alpha matte with all zeros
*/
am_type create_amatte(int rows, int cols);

/**
* Creates an alpha matte using the two images.
* 
* The foregound is the image with the green screen,
* while the backgound is the image to place into the green screen
*/
am_type blue_screen(CGrImage* foreground, CGrImage* backgound, double a1, double a2);

/**
* Applies the alpha matte to the images,
* and stores the result in 'out'
* 
* The first image is the foreground, the second image is the background
*/
void alpha_apply(am_type alpha_matte, CGrImage* first, CGrImage* second, CGrImage* out);
