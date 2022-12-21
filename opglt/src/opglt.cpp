 #include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "renderer.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include <fstream>
#include <sstream>

// \t(gl.+);
// \tGLCall($1);
// 替换
static uint32 ComplierShader(uint32 type,const std::string& source ) {
	auto id = glCreateShader(type);
	auto src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int l;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &l));
		char* message = (char*) alloca(l * sizeof(char));
		GLCall(glGetShaderInfoLog(id, l,&l, message));
		auto t = (type == GL_VERTEX_SHADER) ? "vetex" : "fragment";
		std::cout << "failed to compile" << t << std::endl;
		std::cout << message << std::endl;

	}
	return id;
}
static uint32 CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	auto prog = glCreateProgram();
	auto vs = ComplierShader(GL_VERTEX_SHADER,vertexShader);
	auto fs = ComplierShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(prog, vs));
	GLCall(glAttachShader(prog, fs));
	GLCall(glLinkProgram(prog));
	GLCall(glValidateProgram(prog));


	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
	uint32 indices[] = {
		0,1,3,
		0,3,2
	};
	uint32 vao;
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));
	{ //[1]
		VertexBuffer vb(pos, sizeof(float) * 4 * 2);
		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
		IndexBuffer ib(indices, sizeof(uint32) * 6);

		ShaderSource ss;
		ParseShader(ss, "res/shader/basic.shader");
		auto shader = CreateShader(ss.vs, ss.fs);
		GLCall(glUseProgram(shader));
		float r = 0.0f;
		GLCall(int loc = glGetUniformLocation(shader, "u_Color"));
		ASSERT(loc != -1);
		GLCall(glUniform4f(loc, 0.2f, .3f, .8f, 1.0f));
		auto icr = 0.02f;
		// unbind index buffer (from the last video).
		ib.Unbind();
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glUniform4f(loc, r, .3f, .8f, 1.0f));

			ib.Bind();


			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			if (r >= 1) {
				icr = -0.02f;
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
	} // [1]
	glfwTerminate();
	//[1]
	// vb and ib is deconstructed here, but glDelete is called, and the stack has been destoryed.
	// Solution a: alloc them on the heap.
	// Solution b: manage their lifetime and deconstruct them before DeleteProgram by using a {} (showed above).
	

	return 0;
}