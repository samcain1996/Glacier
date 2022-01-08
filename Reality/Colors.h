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

	const float Red[4]    = { 1.0f, 0.0f, 0.0f, 1.0f };
	const float Green[4]  = { 0.0f, 1.0f, 0.0f, 1.0f };
	const float Blue[4]   = { 0.0f, 0.0f, 1.0f, 1.0f };
	const float Yellow[4] = { 1.0f, 1.0f, 0.0f, 1.0f };
	const float Purple[4] = { 1.0f, 0.0f, 1.0f, 1.0f };
	const float Cyan[4]	  = { 0.0f, 1.0f, 1.0f, 1.0f };
	const float Black[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float White[4]  = { 1.0f, 1.0f, 1.0f, 1.0f };

	void RotateColor(Color& color, const Vec3f& rotator);
	void RotateColor_s(Color& color, const Vec3f& rotator);
}
