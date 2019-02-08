#pragma once
#include "WorldElement.h"
class Field :public WorldElement
{
public:
	void createElement();
	void draw(GLint, GLint);
};

