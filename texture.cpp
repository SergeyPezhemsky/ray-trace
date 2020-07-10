#include "texture.h"

using namespace HydraLiteMath;

float3 ImageTexture::value(float u, float v) const {
	int i = (u)*nx;
	int j = (1 - v) * ny - 0.001;

	//if (i > nx - 1) i = nx - 1;
	//if (j > ny - 1) j = ny - 1;
	if (i < 0) {
		i = abs(abs(i) - nx/10);
	}
	if (j < 0) {
		j = abs(abs(j) - ny/10);
	}
	i = i % nx;
	j = j % ny;

	float r = int(data[3 * i + 3 * nx * j]) / 255.0;
	float g = int(data[3 * i + 3 * nx * j + 1]) / 255.0;
	float b = int(data[3 * i + 3 * nx * j + 2]) / 255.0;

	return float3(r, g, b);
}