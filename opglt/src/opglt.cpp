#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>

static unsigned int ComplierShader(unsigned int type,const std::string& source ) {
	auto id = glCreateShader(type);
	auto src = source.c_str(); // src的生命周期与source一致. eqt : &source[0]
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	//todo:err
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

struct ShaderSource {
	std::string vs;
	std::string fs;
};

static void ParseShader(ShaderSource& s, const std::string& fpath) {
	std::ifstream fs(fpath);

	enum class shaderT {
		None = -1, VERTEX = 0, FRAG = 1
	};

	std::string line;
	std::stringstream ss[2];
	auto type = shaderT::None;
	while (getline(fs, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex")!=std::string::npos)  {
				type = shaderT::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = shaderT::FRAG;
			}
			
		}
		else {
			if (type == shaderT::None) {
				std::cout << "err";
				break;
			}
			ss[(int)type] << line << "\n";

		}
	}
	s.fs = ss[1].str();
	s.vs = ss[0].str();

}







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
	float pos[12] = {
		-0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f,
	};
	
	unsigned int buffer; // id stored here.
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer); // (GLsizei n, GLuint* buffers);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*12,pos,GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),(const void*)0);
	// attribute index; #components; type; need_normalize; 连续顶点属性之间的字节偏移量(sizeof a point);到的第一个components的偏移量
	// make shader
	
	ShaderSource ss;
	ParseShader(ss,"res/shader/basic.shader");
		auto shader = CreateShader(ss.vs, ss.fs);
		glUseProgram(shader);
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glDeleteProgram(shader);
	glfwTerminate();
	return 0;
}