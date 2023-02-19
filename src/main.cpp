#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "particle.h"


#define NUM_PARTICLES 25

#define R_MIN 3
#define R_MAX 50
#define FORCE 100
#define NEG_FORCE 100

int window_width = 720;
int window_height = 480;

float delta_time = 0.0f;
float last_frame = 0.0f;

std::vector<Particle> particles;

void framebuffer_size_update(GLFWwindow* window, int width, int height);
glm::vec2 calculate_attraction(int i);

int main()
{
	// Initialize GLFW and set window hints
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Particle-Life", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Load opengl functions (glad)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initalize GLAD" << std::endl;
		return -1;
	}

	// Set viewport and window resize callback
	glfwGetWindowSize(window, &window_width, &window_height);
	framebuffer_size_update(window, window_width, window_height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_update);

	// -------------------
	// Create shader class
	Shader particle_shader("resources/particle_shader.vert", "resources/particle_shader.frag");

	particle_shader.use();
	particle_shader.setMat4("projection", glm::ortho(0.0, 100.0, 0.0, 100.0));

	srand(time(NULL));
	for (int i = 0; i < NUM_PARTICLES; i++)
	{
		int x = rand() % 100;
		int y = rand() % 100;

		Color color = (Color)(rand() % 6);

		particles.push_back(Particle(glm::vec2(x, y), color));
	}
	
	// Simulation loop
	while (!glfwWindowShouldClose(window))
	{
		float current_frame = glfwGetTime();
		delta_time = current_frame - last_frame;
		last_frame = current_frame;

		glClearColor(0.14, 0.21, 0.25, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);

		for (int i = 0; i < particles.size(); i++)
		{
			glm::vec2 acceleration = calculate_attraction(i);

			particles[i].accelerate(acceleration);
			particles[i].update_position(0.01);
			particles[i].render(particle_shader);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

glm::vec2 calculate_attraction(int i)
{	
	glm::vec2 attraction = glm::vec2(0.0);
	Particle this_particle = particles[i];
	
	for (int j = 0; j < particles.size(); j++)
	{
		if (j == i) continue;

		Particle other_particle = particles[j];

		glm::vec2 force_dir = glm::normalize(this_particle.position - other_particle.position);		
		float distance = glm::length(this_particle.position - other_particle.position);
		float force;

		if (distance < R_MIN)
		{
			force = (((NEG_FORCE / R_MIN) * distance) - NEG_FORCE);
		}
		else if (distance < R_MAX)
		{
			float a = -(FORCE / (0.5*(R_MAX - 2 * R_MIN + 1)));
			float b = (R_MAX + 1) / 2;
			float c = FORCE;
			force = a * abs(distance - b) + c;
		}
		else
		{
			force = 0;
		}

		attraction += -force * force_dir;
	}

	return attraction;
}

void framebuffer_size_update(GLFWwindow* window, int width, int height)
{
	if (width >= height)
	{
		int padding = (width - height) / 2;
		glViewport(padding, 0, height, height);
	}
	else
	{
		int padding = (height - width) / 2;
		glViewport(0, padding, width, width);
	}
}
