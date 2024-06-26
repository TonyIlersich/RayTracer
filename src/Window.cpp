#include "Window.h"
#include "utils.h"
using namespace std;
using namespace glm;

unordered_map<GLFWwindow*, Window*> Window::windowLookup;

void Window::onFrameBufferSizeChange(GLFWwindow* glfwWindow, int width, int height)
{
	Window* window = windowLookup[glfwWindow];
	window->size = ivec2(width, height);
	window->didSizeChangeThisFrame = true;
	glViewport(0, 0, width, height);
}

Window::Window():
	wrapped(nullptr),
	size(800, 600),
	didSizeChangeThisFrame(false),
	keyStates{KF_NONE},
	mouseDelta(0.f)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	wrapped = glfwCreateWindow(size.x, size.y, "Ray Tracer", nullptr, nullptr);
	if (wrapped == nullptr)
	{
		cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		throw runtime_error("Failed to create GLFW window");
	}
	windowLookup[wrapped] = this;
	glfwMakeContextCurrent(wrapped);
	glfwSetFramebufferSizeCallback(wrapped, &Window::onFrameBufferSizeChange);

	glfwSetInputMode(wrapped, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Lock cursor to window
	glfwSetCursorPos(wrapped, 0.0, 0.0);
	if (!glfwRawMouseMotionSupported())
	{
		cerr << "Raw mouse motion not supported\n";
		glfwTerminate();
		throw runtime_error("Raw mouse motion not supported");
	}
	glfwSetInputMode(wrapped, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	glfwSwapInterval(1); // Vsync interval

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		throw runtime_error("Failed to initialize GLAD");
	}
}

Window::~Window()
{
	if (wrapped != nullptr)
	{
		windowLookup.erase(wrapped);
	}
	glfwTerminate();
}

void Window::pollEvents()
{
	didSizeChangeThisFrame = false;

	glfwPollEvents();
	updateKeyStates();
	updateMouseState();

	if (getKeyDown(GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(wrapped, true);
	}
}

void Window::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::display()
{
	glfwSwapBuffers(wrapped);
}

bool Window::shouldClose() const
{
	return glfwWindowShouldClose(wrapped);
}

const ivec2& Window::getSize() const
{
	return size;
}

bool Window::getDidSizeChangeThisFrame() const
{
	return didSizeChangeThisFrame;
}

Window::KeyFlags Window::getKeyState(int key) const
{
	return keyStates[key];
}

bool Window::getKeyDown(int key) const
{
	return keyStates[key] & KF_DOWN;
}

bool Window::getKeyFresh(int key) const
{
	return keyStates[key] & KF_FRESH;
}

const vec2& Window::getMouseDelta() const
{
	return mouseDelta;
}

void Window::updateKeyStates()
{
	for (int key = 0; key <= GLFW_KEY_LAST; key++)
	{
		if (glfwGetKey(wrapped, key) == GLFW_PRESS) // The key is down
		{
			if (keyStates[key] & KF_DOWN) // The key was already down
			{
				keyStates[key] &= ~KF_FRESH; // So the key is no longer fresh
			}
			else // The key was previously up
			{
				keyStates[key] |= KF_DOWN | KF_FRESH; // So the key is now down and fresh
			}
		}
		else // The key is up
		{
			if (keyStates[key] & KF_DOWN) // The key was previously down
			{
				keyStates[key] = (keyStates[key] & ~KF_DOWN) | KF_FRESH; // So the key is now up and fresh
			}
			else // The key was already up
			{
				keyStates[key] &= ~KF_FRESH; // So the key is no longer fresh
			}
		}
	}
}

void Window::updateMouseState()
{
	double dx, dy;
	glfwGetCursorPos(wrapped, &dx, &dy);
	mouseDelta = vec2(dx, dy);
	glfwSetCursorPos(wrapped, 0.0, 0.0);
}
