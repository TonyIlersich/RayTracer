#include "Game.h"
#include "SceneFactory.h"
#include "Timer.h"
#include "utils.h"
using namespace std;
using namespace glm;

static constexpr char* commonShaderSource =
#include "shaders/common.glsl"
"";

static constexpr char* vertSource =
// #include "shaders/vertex.glsl"
#include "shaders/rt_vertex.glsl"
"";

static constexpr char* fragSource =
// #include "shaders/fragment.glsl"
#include "shaders/rt_fragment.glsl"
"";

// static constexpr char* computeSource =
// #include "shaders/compute.glsl"
// ;

#define LOG_FPS

static const void* getBufferData(const Scene& scene, GLsizeiptr& size)
{
	size = 0; // TODO
	return nullptr;
}

Game::Game():
	scene(nullptr),
	eye(radians(45.f)) // TODO: this should probably be initialized elsewhere
{
	eyeController.setEye(&eye);
	eyeController.setSensitivity(-0.0003f);
	eyeController.moveCamera(upward * 2.f, vec2(0));

	shaderProgram.addShader(vertSource, GL_VERTEX_SHADER);
	shaderProgram.addShader(commonShaderSource, GL_FRAGMENT_SHADER);
	shaderProgram.addShader(fragSource, GL_FRAGMENT_SHADER);
	shaderProgram.link();
	if (shaderProgram.getIsBad())
	{
		cerr << "Shader program is bad\n";
		throw runtime_error("Shader program is bad");
	}

	scene = SceneFactory::createSampleScene();
	screenMesh.bind();
	screenMesh.setAttributes({ 2 });
	screenMesh.setVertices({
		// position
		 1.0f,  1.0f, // top right
		 1.0f, -1.0f, // bottom right
		-1.0f, -1.0f, // bottom left
		-1.0f,  1.0f, // top left
	});
	screenMesh.setIndices({
		0, 1, 3,
		1, 2, 3,
	});

	shaderProgram.bind();
	shaderProgram.setUniform("fovy", eye.getFov());
	const ivec2 windowSize = window.getSize();
	shaderProgram.setUniform("aspect", float(windowSize.x) / windowSize.y);
	shaderProgram.setUniform("eyeTrans", eye.getTrans());

	// Bind uniform block
	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram.id, "SceneBlock");
	glUniformBlockBinding(shaderProgram.id, blockIndex, 0);

	// Create uniform buffer object
	GLuint bufferId;
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);
	glBufferData(GL_UNIFORM_BUFFER, 16384, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Bind ubo
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, bufferId, 0, 16384);

	// Get byte offsets of uniforms in block
	const char* names[]
	{
		"worldToLocal",
		"surfaceType",
		"surfaceMaterialId",
		"materialEmission",
		"materialAlbedo",
		"materialGlossiness",
		"surfaceCount"
	};
	constexpr auto count = sizeof(names)/sizeof(names[0]);
	GLuint indices[count];
	// while (glGetError());
	glGetUniformIndices(shaderProgram.id, count, names, indices);
	// cerr << "error: " << glGetError() << '\n';
	// cerr << indices[0] << ", " << indices[1] << ", " << indices[2] << ", " << indices[3] << ", " << indices[4] << ", " << indices[5] << ", " << indices[6] << '\n';

	GLint offsets[count];
	// while (glGetError());
	glGetActiveUniformsiv(shaderProgram.id, count, indices, GL_UNIFORM_OFFSET, offsets);
	// cerr << "error: " << glGetError() << '\n';
	// cerr << offsets[0] << ", " << offsets[1] << ", " << offsets[2] << ", " << offsets[3] << ", " << offsets[4] << ", " << offsets[5] << ", " << offsets[6] << '\n';

	glBindBuffer(GL_UNIFORM_BUFFER, bufferId);

	mat4 worldToLocal[64];
	GLuint surfaceType[64];
	GLuint surfaceMaterialId[64];
	vec4 materialEmission[16];
	vec4 materialAlbedo[16];
	float materialGlossiness[16];
	GLuint surfaceCount;

	unordered_map<const Material*, GLuint> materialIdTable;

	scene->serializeForShader(
		worldToLocal, surfaceType, surfaceMaterialId, materialEmission, materialAlbedo, materialGlossiness, surfaceCount,
		materialIdTable);
	// cerr << "surfaces: " << surfaceCount << '\n';

	// while (glGetError());
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[0], sizeof(worldToLocal), &worldToLocal);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[1], sizeof(surfaceType), &surfaceType);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[2], sizeof(surfaceMaterialId), &surfaceMaterialId);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[3], sizeof(materialEmission), &materialEmission);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[4], sizeof(materialAlbedo), &materialAlbedo);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[5], sizeof(materialGlossiness), &materialGlossiness);
	// cerr << "error: " << glGetError() << '\n';
	glBufferSubData(GL_UNIFORM_BUFFER, offsets[6], sizeof(surfaceCount), &surfaceCount);
	// cerr << "error: " << glGetError() << '\n';

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// const void* data = getBufferData(scene, dataSize);
	// delete data;
	// data = nullptr;
	// glGetUniformIndices(shaderProgram.id, );

	// Old compute shader setup
	// glGenTextures(1, &texId);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, texId);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// ivec2 size = window.getSize();
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, size.x, size.y, 0, GL_RGBA, GL_FLOAT, NULL);
	// glBindImageTexture(0, texId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
}

Game::~Game()
{
	delete scene;
}

void Game::run()
{
#ifdef LOG_FPS
	Timer frameRateTimer;
#endif
	while (!window.shouldClose())
	{
		update();
		render();
#ifdef LOG_FPS
		cerr << "\rframerate: " << setw(7) << fixed << setprecision(1) << 1.f / frameRateTimer.lap();
#endif
		window.pollEvents();
	}
}

void Game::update()
{
	// Move eye
	vec3 velocity(
		window.getKeyDown(GLFW_KEY_D)     - window.getKeyDown(GLFW_KEY_A),
		window.getKeyDown(GLFW_KEY_SPACE) - window.getKeyDown(GLFW_KEY_LEFT_CONTROL),
		window.getKeyDown(GLFW_KEY_S)     - window.getKeyDown(GLFW_KEY_W));
	if (velocity != vec3(0) || window.getMouseDelta() != vec2(0))
	{
		eyeController.moveCamera(velocity / 60.f, window.getMouseDelta());
		shaderProgram.bind();
		shaderProgram.setUniform("eyeTrans", eye.getTrans());
	}
}

void Game::render()
{
	// computeShader.bind();
	// ivec2 windowSize = window.getSize();
	// glDispatchCompute((unsigned int)windowSize.x, (unsigned int)windowSize.y, 1);
	// // make sure writing to image has finished before read
	// glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

	// window.clear();
	shaderProgram.bind();
	shaderProgram.setUniform("time", gameTime.getTime());
	// shaderProgram.setUniform("tex", 0);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, texId);
	screenMesh.bind();
	screenMesh.draw();
	window.display();
}
