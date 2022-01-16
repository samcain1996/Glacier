/**
 * @file Colors.h
 * @author Sam Cain (samuelrcain@gmail.com)
 * @brief Defines a list of colors for use in uniforms for shaders
 * @version 0.1
 * @date 2022-01-02
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once
#include "Defs.h"

namespace colors {

	// Some basic colors
	const float Red[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
	const float Green[4]  = { 0.0f, 1.0f, 0.0f, 1.0f };
	const float Blue[4]   = { 0.0f, 0.0f, 1.0f, 1.0f };
	const float Yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	const float Purple[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	const float Cyan[4]	  = { 0.0f, 1.0f, 1.0f, 1.0f };
	const float Black[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float White[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };

	typedef struct Color {
		float rgba[4];

		Color& operator+=(const Vec3f& rhs)
		{
			rgba[0] += rhs.x;
			rgba[1] += rhs.y;
			rgba[2] += rhs.z;
			return *this;
		}

		Color& operator-=(const Vec3f& rhs)
		{
			rgba[0] -= rhs.x;
			rgba[1] -= rhs.y;
			rgba[2] -= rhs.z;
		}

		Color(const float color[4])
		{
			rgba[0] = color[0];
			rgba[1] = color[1];
			rgba[2] = color[2];
			rgba[3] = color[3];
		}

		operator float* () {
			return rgba;
		}

	} Color;
	/**
	 * @brief 				Change color by amount in rotator
	 * 						no bounds checking
	 * 
	 * @param color 		color to change
	 * @param rotator 		r, g, and b values to change by
	 */
	void RotateColor(Color& color, const Vec3f& rotator);

	// Same as above with bounds checking
	void RotateColor_s(Color& color, const Vec3f& rotator);
}
