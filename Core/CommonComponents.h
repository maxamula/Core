#pragma once
#include "Vector.h"
#include <string>

struct Tag
{
	std::string tag;
};

struct Transformation
{
	vec3 position;
	vec3 rotation;
	vec3 scaling;
};

