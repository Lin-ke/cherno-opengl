#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<string>
#include <iostream>

static unsigned int ComplierShader(unsigned int type,const std::string& source ) {
	auto id = glCreateShader(type);
	auto src = source.c_str(); // src的生命周期与source一致. eqt : &source[0]
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//todo:err handle.
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int l;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l);
		char* message = (char*) alloca(l * sizeof(char));
		glGetShaderInfoLog(id, l,&l, message);
		auto t = (type == GL_VERTEX_SHADER) ? "vetex" : "fragment";
		std::cout << "failed to compile" << t << std::endl;
		std::cout << message << std::endl;

	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	auto prog = glCreateProgram();
	auto vs = ComplierShader(GL_VERTEX_SHADER,vertexShader);
	auto fs = ComplierShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);
	glValidateProgram(prog);


	glDeleteShader(vs);
	glDeleteShader(fs);

	return prog;
}







// Advise:
// Watch p10 and learn to handle errors if you feel hard debugging.


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
	if (glewInit() != GLEW_OK) {
		std::cout << " error";
		return -1;
	}
	std::cout << glGetString(GL_VERSION);
	float pos[6] = {
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, 0.5f,
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6,pos,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float),(const void*) 0);

	//shader code begin:
	std::string vetexShader =
		"#version 330 core\n" 
		"\n"
		"layout(location = 0) in vec4 position;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = position;\n"
		"}\n";
	// RGBA
	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout(location = 0) out vec4 color;\n"
		"void main()\n"
		"{\n"
		"	color = vec4(1.0,0.0,0.0,1.0);\n"
		"}\n";
	
		auto shader = CreateShader(vetexShader, fragmentShader);
		glUseProgram(shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}