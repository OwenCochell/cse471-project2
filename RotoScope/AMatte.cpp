#include "AMatte.h"

am_type create_amatte(int rows, int cols)
{
	// Create the map:
	auto am = am_type();

	// Set the number of rows:

	am->reserve(rows);

	// Iterate over number of rows:

	for (int i = 0; i < rows; ++i) {

		// Create a new vector:

		std::vector<double> col(cols);

		// Add to final map:

		am->push_back(col);
	}

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

			BYTE fblue = *foreground[r][c * 3 + 2];
			BYTE fgreen = *foreground[r][c * 3 + 1];

			double alpha = 1 - a1 * (fblue - a2*fgreen);

			// Determine if we need to clamp:

			if (alpha < 0) {
				alpha = 0;
			}
			if (alpha > 1) {
				alpha = 1;
			}

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

	out->SetSize(col, row);

	// Iterate over alpha_matte:

	for (int r = 0; r < row; ++r) {
		for (int c = 0; c < col; ++c) {

			// Get the alpha value:

			double alpha = alpha_matte->at(r).at(c);

			// Determine colors:
			// C0 = (1-a)C1 + a*C2

			BYTE r = (1 - alpha) * (*first[r][c * 3]) + alpha * (*second[r][c * 3]);
			BYTE g = (1 - alpha) * (*first[r][c * 3 + 1]) + alpha * (*second[r][c * 3 + 1]);
			BYTE b = (1 - alpha) * (*first[r][c * 3 + 2]) + alpha * (*second[r][c * 3 + 2]);

			// Set the colors:

			out->Set(c, r, r, g, b);

		}
	}
}
