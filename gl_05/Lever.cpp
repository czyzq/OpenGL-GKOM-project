#include "Lever.h"
#include <math.h>
#pragma once

const int WALLS = 20;
const int VERTICES_COUNT = 3 * WALLS * 30; // splain.length * N * 30    .. + N * 2 * 15
// 9 * 5 * 30 

void Lever::createElement() {

	GLfloat vertices[VERTICES_COUNT];

	std::pair<GLfloat, GLfloat> splain[] = {
		{ 0.0f, 0.0f },
		{ 0.05f, 0.2f },
		{ 1.4f, 0.2f },
		{ 1.54f, 0.0f }
	
	};


	int splainLength = sizeof(splain) / sizeof(*splain);
	int j = 0;
	GLfloat prev_sin = std::sin(glm::radians(360.0f * (-1) / WALLS));
	GLfloat prev_cos = std::cos(glm::radians(360.0f * (-1) / WALLS));

	for (int k = 0; k < WALLS; k++) { // przyblizenie przez N scian
		GLfloat sin = std::sin(glm::radians(360.0f * k / WALLS));
		GLfloat cos = std::cos(glm::radians(360.0f * k / WALLS));
		for (int i = 1; i < splainLength; i++) {

			// prawy dolny
			vertices[j] = prev_cos * splain[i - 1].second;
			vertices[j + 1] = splain[i - 1].first;
			vertices[j + 2] = prev_sin * splain[i - 1].second;
			vertices[j + 3] = vertices[j + 1];
			vertices[j + 4] = vertices[j] / 3;

			for (int temp = 0; temp < 2; temp++) {
				// prawy gorny
				j += 5;
				vertices[j] = prev_cos * splain[i].second;
				vertices[j + 1] = splain[i].first;
				vertices[j + 2] = prev_sin * splain[i].second;
				vertices[j + 3] = vertices[j + 1];
				vertices[j + 4] = vertices[j] / 3;

				//lewy dolny
				j += 5;
				vertices[j] = cos * splain[i - 1].second;
				vertices[j + 1] = splain[i - 1].first;
				vertices[j + 2] = sin * splain[i - 1].second;
				vertices[j + 3] = vertices[j + 1];
				vertices[j + 4] = vertices[j] / 3;
			}
			j += 5;
			vertices[j] = cos * splain[i].second;
			vertices[j + 1] = splain[i].first;
			vertices[j + 2] = sin * splain[i].second;
			vertices[j + 3] = vertices[j + 1];
			vertices[j + 4] = vertices[j] / 3;

			j += 5;
		}
		prev_sin = sin;
		prev_cos = cos;

	}
	std::cout << j;



	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);

	this->VAO = VAO;
	this->VBO = VBO;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO


	// --== TEXTURE 1 == --
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image("textures/dark-wood.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	this->texture1 = texture1;
}

void Lever::draw(GLint modelLoc, GLint textureLoc)
{

	GLfloat timeValue = fmod((WORLD_SPEED * glfwGetTime() * 2.0), 50.0f);
	GLfloat swing = std::fminf(1.0f, std::fmaxf(0.0f, (timeValue - 14.45f) / 3.5f));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->getTexture1());
	glUniform1i(textureLoc, 0);
	glBindVertexArray(this->getVAO());

	glm::mat4 model = glm::mat4();
	model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(3.5f, 3.0f, 3.5f));
	model = glm::translate(model, glm::vec3(0.0f, -0.8f, -1.35f));
	model = glm::translate(model, glm::vec3(6.0f, 0.65f, 0.0f));
	model = glm::rotate(model, glm::radians(11.75f*swing), glm::vec3(-1.0f, 0.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-6.0f, -0.65f, 0.0f));

	model = glm::scale(model, glm::vec3(0.1f, 0.5f, 0.1f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, VERTICES_COUNT / 5);
	glBindVertexArray(0);
}

