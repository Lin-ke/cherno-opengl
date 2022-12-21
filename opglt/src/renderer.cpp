#include "renderer.h"
void GLClearError() {
	while (glGetError() != GL_NO_ERROR) {
	}
}
bool GLLogCall(const char* function, const char* filename, int line) {
	while (GLenum error = glGetError()) {
		std::cout << "error : " << function << " line:" << line << " error: " << error << " " << std::endl;
		return false;
	}
	return true;
}

