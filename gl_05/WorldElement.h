#include <iostream>
#include <vector>

#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

const int MAX_VERTICES_AMOUNT = 3000;

const GLfloat WORLD_SPEED = 3.0;

#pragma once
class WorldElement
{
public:
	GLuint getVBO();
	GLuint getVAO();
	GLuint getTexture1();
	GLuint getTexture2();
	virtual void createElement() = 0;
	virtual void draw(GLint, GLint) = 0;

protected:
	GLuint VBO, VAO;
	GLuint texture1;
	GLuint texture2;
};

