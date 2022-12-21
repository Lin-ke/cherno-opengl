#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1; 

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	auto v = glewInit();
	if (v != GLEW_OK) {
		std::cout << " error";
	}
	std::cout << glGetString(GL_VERSION);
	float pos[12] = {
		0.5f, 0.5f,0.5f, 0.5f,
		0.5f, -0.5f,0.5f, -0.5f,
		-0.5f, 0.5f,-0.5f, 0.5f,
	};

	unsigned int buffer; // id stored here.
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // (GLsizei n, GLuint* buffers);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12,pos,GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),(const void*)8);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_TRIANGLES);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}