#include "pch.h"

#include "AMatte.h"

am_type create_amatte(int rows, int cols)
{
	// Create the map:

	am_type am = std::make_unique<std::vector<std::vector<double>>>(std::vector<std::vector<double>>(rows, std::vector<double>(cols)));

	// Return the matte:

	return am;
}

am_type blue_screen(CGrImage* foreground, CGrImage* backgound, double a1, double a2)
{
	// Create the empty matte:

	int rows = foreground->GetHeight();
	int cols = foreground->GetWidth();

	auto matte = create_amatte(rows, cols);

	// Iterate over the rows and cols:

	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {

			// Compute the alpha value:
			// B, G, R

			BYTE fblue = (*foreground)[r][c * 3];
			BYTE fgreen = (*foreground)[r][c * 3 + 1];

			//double alpha = 1 - a1 * (fblue - a2*fgreen);
			double alpha = 1 - a1 * (fgreen - a2 * fblue);
			//double alpha = 1 - a1 * ((std::min)(fgreen, fblue) - a2 * fblue);

			// Determine if we need to clamp:

			if (alpha < 0.) {
				alpha = 0.;
			}
			if (alpha > 1.) {
				alpha = 1.;
			}
			//if (0 > alpha) {
			//	alpha = 1.;
			//}

			// Set the alpha value:

			matte->at(r).at(c) = alpha;
		}
	}

	// Finally, return the alpha matte:

	return matte;
}

void alpha_apply(am_type alpha_matte, CGrImage* first, CGrImage* second, CGrImage* out)
{
	// Size out image:

	int row = alpha_matte->size();
	int col = alpha_matte->at(0).size();

	int row2 = first->GetHeight();
	int col2 = first->GetWidth();

	out->SetSize(col, row);

	// Iterate over alpha_matte:

	for (int r = 0; r < row && r < row2; ++r) {
		for (int c = 0; c < col && c < col2; ++c) {

			// Get the alpha value:

			double alpha = alpha_matte->at(r).at(c);

			// Determine colors:
			// // B, G, R
			// C0 = (1-a)C1 + a*C2

			BYTE red = (1 - alpha) * (*first)[r][c * 3 + 2] + alpha * (*second)[r][c * 3 + 2];
			BYTE green = (1 - alpha) * (*first)[r][c * 3 + 1] + alpha * (*second)[r][c * 3 + 1];
			BYTE blue = (1 - alpha) * (*first)[r][c * 3] + alpha * (*second)[r][c * 3];

			// Set the colors:

			out->Set(c, r, red, green, blue);

		}
	}
}
