/**
 * @file Defs.h
 * @author Sam Cain (samuelrcain@gmail.com)
 * @brief  Custom types used throughout project
 * @version 0.1
 * @date 2022-01-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once

typedef struct Vec3f 
{
	float x;
	float y;
	float z;
} Vec3f;

typedef struct vec4f
{
	float w;
	float x;
	float y;
	float z;
} vec4f;

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