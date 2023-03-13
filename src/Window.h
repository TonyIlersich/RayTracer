#ifndef WINDOW_H
#define WINDOW_H

#include "includes.h"
#include "Eye.h"
#include "Scene.h"

class Window
{
public:
	typedef uint8_t KeyFlags;
	constexpr static KeyFlags KF_NONE  = 0x00;
	constexpr static KeyFlags KF_DOWN  = 0x01;
	constexpr static KeyFlags KF_FRESH = 0x02;

	Window();
	~Window();

	void pollEvents();
	void clear();
	void display();
	
	KeyFlags getKeyState(int key) const;
	bool getKeyDown(int key) const;
	bool getKeyFresh(int key) const;

	const glm::vec2& getMouseDelta() const;

	glm::ivec2 getSize() const;

	bool shouldClose() const;

private:
	void updateKeyStates();
	void updateMouseState();

	GLFWwindow* wrapped; // ownership
	KeyFlags keyStates[GLFW_KEY_LAST + 1];
	glm::vec2 mouseDelta;
};

#endif