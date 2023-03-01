#ifndef WINDOW_H
#define WINDOW_H

#include "includes.h"

class Window
{
public:
	Window();
	~Window();

	void update();
	bool shouldClose() const;

private:
	void processInput();

	GLFWwindow* wrapped;
};

#endif