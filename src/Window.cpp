#include "Window.h"
using namespace std;

// TODO: should this be a member function?
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Window::Window(): wrapped(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	wrapped = glfwCreateWindow(800, 600, "Ray Tracer", nullptr, nullptr);
	if (wrapped == nullptr)
	{
		cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		throw runtime_error("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(wrapped);
	glfwSetFramebufferSizeCallback(wrapped, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		throw runtime_error("Failed to initialize GLAD");
	}
}

Window::~Window()
{
	glfwTerminate();
}

void Window::update()
{
	processInput();

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(wrapped);
	glfwPollEvents();
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(wrapped);
}

void Window::processInput()
{
	if(glfwGetKey(wrapped, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(wrapped, true);
	}
}
