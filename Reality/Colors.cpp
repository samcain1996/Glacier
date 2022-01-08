#include "Colors.h"

void colors::RotateColor(Color& color, const Vec3f& rotator)
{
	color[0] += rotator.x;
	color[1] += rotator.y;
	color[2] += rotator.z;
}

void colors::RotateColor_s(Color& color, const Vec3f& rotator)
{
	RotateColor(color, rotator);

	// Bounds checking
	if (color[0] > 1.0f) { color[0] -= 1; }
	if (color[1] > 1.0f) { color[1] -= 1; }
	if (color[2] > 1.0f) { color[2] -= 1; }
}
