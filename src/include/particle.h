#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"


enum Color
{
	blue,
	green,
	cyan,
	red,
	magenta,
	yellow
};

class Particle
{
public:
	glm::vec2 position;
	Color particle_color;

	Particle(glm::vec2 _position, Color _color);

	void update_position(float delta_time);
	void accelerate(glm::vec2 acc);
	void render(Shader shader);

private:
	inline static bool VAO_initialized;
	inline static unsigned int VAO, VBO;
	static void initialize_VAO();

	glm::mat4 model_mat;
	
	glm::vec2 position_last;
	glm::vec2 acceleration;

	void wrap_position();
};
