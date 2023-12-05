#include "pch.h"

#include "ImageWarp.h"

#include <cmath>

void RippleTans(int x2, int y2, int width, int height, double& x1, double& y1)
{

	int w = 0;
	int h = 0;

	double dc = sqrt(double((x2 - w / 2) * (x2 - w / 2) +
		(y2 - h / 2) * (y2 - h / 2)));
	// Offset amount?
	double offset = sin(dc / 50. * 6.2830) * 20;
	// This is the equivalent point in the source image
	double dc1 = dc + offset;
	x1 = w / 2 + dc1 / dc * (x2 - w / 2);
	y1 = h / 2 + dc1 / dc * (y2 - h / 2);
}

BYTE interpolate(double x, double y, BYTE tl, BYTE tr, BYTE bl, BYTE br) {

    double fractX = x - static_cast<int>(x);
    double fractY = y - static_cast<int>(y);

    BYTE top = tl + static_cast<BYTE>(fractX * (tr - tl));
    BYTE bottom = bl + static_cast<BYTE>(fractX * (br - bl));

    return top + static_cast<BYTE>(fractY * (bottom - top));

}

void applyRippleEffect(CGrImage& image, float time, float frequency=0.01, float amplitude=1) {
    //float frequency = 0.01;  // Adjust the frequency of the ripple (original - 0.1)
    //float amplitude = 1; // Adjust the amplitude of the ripple (original 10)

    // Get the width and height:

    int width = image.GetWidth();
    int height = image.GetHeight();

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double displacement = amplitude * sin(2 * 3.14159 * frequency * sqrt(pow(x - width / 2, 2) + pow(y - height / 2, 2)) - 0.1 * time);

            double newX = x + displacement;
            double newY = y + displacement;

            if (newX >= 0 && newX < width - 1 && newY >= 0 && newY < height - 1) {

                // Non-interpolated:
                //BYTE blue = image[newY][newX * 3];
                //BYTE green = image[newY][newX * 3 + 1];
                //BYTE red = image[newY][newX * 3 + 2];

                //image.Set(x, y, red, green, blue);

                int x1 = static_cast<int>(newX);
                int y1 = static_cast<int>(newY);

                int x2 = x1 + 1;
                int y2 = y1 + 1;

                // New colors
                BYTE nblue = interpolate(newX, newY, image[y1][x1*3], image[y1][x2*3], image[y2][x1*3], image[y2][x2*3]);
                BYTE ngreen = interpolate(newX, newY, image[y1][x1*3+1], image[y1][x2*3+1], image[y2][x1*3+1], image[y2][x2*3+1]);
                BYTE nred = interpolate(newX, newY, image[y1][x1*3+2], image[y1][x2*3+2], image[y2][x1*3+2], image[y2][x2*3+2]);

                image.Set(x, y, nred, ngreen, nblue);

                //image[y][x][0] = interpolate(newX, newY, image[y1][x1][0], image[y1][x2][0], image[y2][x1][0], image[y2][x2][0]);
                //image[y][x][1] = interpolate(newX, newY, image[y1][x1][1], image[y1][x2][1], image[y2][x1][1], image[y2][x2][1]);
                //image[y][x][2] = interpolate(newX, newY, image[y1][x1][2], image[y1][x2][2], image[y2][x1][2], image[y2][x2][2]);

                //image[y][x][0] = image[newY][newX][0];
                //image[y][x][1] = image[newY][newX][1];
                //image[y][x][2] = image[newY][newX][2];
            }
        }
    }

}
