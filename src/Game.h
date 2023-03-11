#ifndef GAME_H
#define GAME_H

#include "includes.h"
// #include "ComputeShader.h"
#include "Mesh.h"
#include "Scene.h"
#include "ShaderProgram.h"
#include "Window.h"

class Game
{
public:
	Game();
	~Game();

	void run();

private:
	void update();
	void render();

	Scene* scene; // ownership
	Eye eye;
	Window window;
	// ComputeShader computeShader;
	ShaderProgram shaderProgram;
	Mesh screenMesh;
	GLuint texId;
};

#endif