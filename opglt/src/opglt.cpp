#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__,__LINE__))
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR) {
	}
}
static bool GLLogCall(const char* function , const char* filename, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "error : " <<function <<" line:"<<line<<" error: "<<error <<" " << std::endl;
		return false;
	}
	return true;
}


static unsigned int ComplierShader(unsigned int type, const std::string& source);
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

struct ShaderSource {
	std::string vs;
	std::string fs;
};
static void ParseShader(ShaderSource& s, const std::string& fpath);

// vertex array object:
// bind vertex  buffer with layout and etc.
// It's default created when use GLFW_OPENGL_COMPAT_PROFILE.

// it's advised to use different vaos rather than use different vertex buffers and specifiers.
// Years ago, it's faster to use one single vao.



int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1; 
	

	// Test: with this, you need to bind every time in the loop.
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		GLCall(glfwTerminate());
		return -1;
	}
	
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	// frame rate
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK) {
		std::cout << " error";
		return -1;
	}
	float pos[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		-0.5f, 0.5f,
		0.5f, 0.5f,
	};
	unsigned int indices[] = {
		0,1,3,
		0,3,2
	};
	unsigned int buffer; 
	unsigned int vao;
	// VAO TEST
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);


	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); 
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*2,pos,GL_STATIC_DRAW));
	// -> [specify vertex layout] but not actually bined with the buffer, which needs to call per time.
	GLCall(glEnableVertexAttribArray(0));

	// Actually : through this code link with vao(index 0 with buffer).
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float),(const void*)0));
	unsigned int ibo; // index buffer
	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo)); 
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*6, indices, GL_STATIC_DRAW));
	ShaderSource ss;
	ParseShader(ss,"res/shader/basic.shader");
	auto shader = CreateShader(ss.vs, ss.fs);
	GLCall(glUseProgram(shader));
	float r = 0.0f;
	GLCall(int loc = glGetUniformLocation(shader, "u_Color"));
	ASSERT(loc != -1);
	GLCall(glUniform4f(loc, 0.2f, .3f, .8f, 1.0f));
	auto icr = 0.02f;
	// after binding vao, though vetex buffer bind upwards are canceled, it will still works.
	// Test:

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		//Test

		glUseProgram(shader);
		GLCall(glUniform4f(loc, r, .3f, .8f, 1.0f));

		// only need to bind vertex array vao
		glBindVertexArray(vao);
		// otherwise, need bind vertex buffer and specify layout.

		//GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); // (GLsizei n, GLuint* buffers));
		//GLCall(glEnableVertexAttribArray(0));
		//GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
		if (r >= 1) {
			icr =- 0.02f;
		}
		else if (r <= 0) {
			icr = 0.02f;
		}
		r += icr;
		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		GLCall(glfwPollEvents());
	}
	GLCall(glDeleteProgram(shader));
	GLCall(glfwTerminate());
	return 0;
}
static unsigned int ComplierShader(unsigned int type, const std::string& source) {
	GLCall(auto id = glCreateShader(type));
	auto src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int l;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l));
		char* message = (char*)alloca(l * sizeof(char));
		GLCall(glGetShaderInfoLog(id, l, &l, message));
		auto t = (type == GL_VERTEX_SHADER) ? "vetex" : "fragment";
		std::cout << "failed to compile" << t << std::endl;
		std::cout << message << std::endl;

	}
	return id;
}
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	auto prog = glCreateProgram();
	auto vs = ComplierShader(GL_VERTEX_SHADER, vertexShader);
	auto fs = ComplierShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(prog, vs));
	GLCall(glAttachShader(prog, fs));
	GLCall(glLinkProgram(prog));
	GLCall(glValidateProgram(prog));


	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return prog;
}
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
			if (line.find("vertex") != std::string::npos) {
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
