#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "particle.h"

#define NUM_VERTICES 100
#define RADIUS 0.4


Particle::Particle(glm::vec2 _position, Color _color)
{
	if (!VAO_initialized)
	{
		initialize_VAO();
	}

	position = _position;
	position_last = position;

	particle_color = _color;

	acceleration = glm::vec2(0.0f);
}

void Particle::update_position(float delta_time)
{
	const glm::vec2 velocity = (position - position_last) * 0.9f;			// 0.9f - slow down progressivly (drag) 
	// save current position
	position_last = position;
	// perform verlet integration
	position = position + velocity + acceleration * delta_time * delta_time;
	// reset acceleration
	acceleration = {};

	wrap_position();

	model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, glm::vec3(position, 0));
}

void Particle::wrap_position()
{
	while (position.x > 100)
	{
		position.x -= 100;
		position_last.x -= 100;
	}
	while (position.x < 0)
	{
		position.x += 100;
		position_last.x += 100;
	}
	while (position.y > 100)
	{
		position.y -= 100;
		position_last.y -= 100;
	}
	while (position.y < 0)
	{
		position.y += 100;
		position_last.y += 100;
	}
}

void Particle::accelerate(glm::vec2 acc)
{
	acceleration = acc;
}

void Particle::render(Shader shader)
{
	shader.use();

	switch (particle_color)
	{
	case red:
		shader.setVec3("color", glm::vec3(1, 0, 0));
		break;
	
	case green:
		shader.setVec3("color", glm::vec3(0, 1, 0));
		break;

	case blue:
		shader.setVec3("color", glm::vec3(0, 0, 1));
		break;

	case magenta:
		shader.setVec3("color", glm::vec3(1, 0, 1));
		break;

	case yellow:
		shader.setVec3("color", glm::vec3(1, 1, 0));
		break;

	case cyan:
		shader.setVec3("color", glm::vec3(0, 1, 1));
		break;

	default:
		shader.setVec3("color", glm::vec3(1, 1, 1));
		break;
	}

	shader.setMat4("model", model_mat);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_VERTICES);
}

void Particle::initialize_VAO()
{
	float twoPi = 2 * M_PI;
	
	float vertices[NUM_VERTICES * 3];

	for (int i = 0; i < NUM_VERTICES; i++)
	{
		float angle = i * twoPi / NUM_VERTICES;

		vertices[(3 * i) + 0] = cos(angle) * RADIUS;			// each vertex is 3 floats (i + 0, i + 1, i + 2)
		vertices[(3 * i) + 1] = sin(angle) * RADIUS;
		vertices[(3 * i) + 2] = 0;
	}

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
