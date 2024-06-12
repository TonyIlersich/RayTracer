#include "Game.h"
#include "SceneFactory.h"
#include "Timer.h"
#include "utils.h"
using namespace std;
using namespace glm;

static constexpr char* commonShaderSource =
#include "shaders/common.glsl"
"";

static constexpr char* rtVertSource =
#include "shaders/rt_vertex.glsl"
"";

static constexpr char* rtFragSource =
#include "shaders/rt_fragment.glsl"
"";

static constexpr char* ppVertSource =
#include "shaders/pp_vertex.glsl"
"";

static constexpr char* ppFragSource =
#include "shaders/pp_fragment.glsl"
"";

#define LOG_FPS

Game::Game():
	scene(nullptr),
	eye(radians(45.f)), // TODO: this should probably be initialized elsewhere
	frameBufferId(-1),
	frameTextureId(-1)
{
	eyeController.setEye(&eye);
	eyeController.setSensitivity(-0.0003f);
	eyeController.moveCamera(upward * 2.f, vec2(0));

	glDisable(GL_DEPTH_TEST); // We won't be needing this

	// Create frame buffer for off-screen rendering
	glGenFramebuffers(1, &frameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	glEnablei(GL_FRAMEBUFFER, GL_BLEND);
	glBlendColor(1.0f, 1.0f, 1.0f, 0.2f);
	glBlendFuncSeparatei(frameBufferId, GL_ONE, GL_CONSTANT_ALPHA, GL_ONE, GL_ZERO);
	// Create texture for frame buffer
	glGenTextures(1, &frameTextureId);
	glBindTexture(GL_TEXTURE_2D, frameTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.getSize().x, window.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameTextureId, 0);
	// Verify frame buffer status
	switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		case GL_FRAMEBUFFER_COMPLETE:
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			cerr << "Frame buffer attachment is incomplete\n";
			throw runtime_error("Frame buffer attachment is incomplete");
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			cerr << "Frame buffer is missing attachment\n";
			throw runtime_error("Frame buffer is missing attachment");
		default:
			cerr << "Frame buffer is incomplete\n";
			throw runtime_error("Frame buffer is incomplete");
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Set up ray-tracing shader
	rtShader.addShader(rtVertSource, GL_VERTEX_SHADER);
	rtShader.addShader(commonShaderSource, GL_FRAGMENT_SHADER);
	rtShader.addShader(rtFragSource, GL_FRAGMENT_SHADER);
	rtShader.link();
	if (rtShader.getIsBad())
	{
		cerr << "Ray-tracing shader program is bad\n";
		throw runtime_error("Ray-tracing shader program is bad");
	}
	rtShader.bind();
	rtShader.setUniform("fovy", eye.getFov());
	rtShader.setUniform("aspect", float(window.getSize().x) / window.getSize().y);
	rtShader.setUniform("eyeTrans", eye.getTrans());

	// Set up post-processing shader
	ppShader.addShader(ppVertSource, GL_VERTEX_SHADER);
	ppShader.addShader(ppFragSource, GL_FRAGMENT_SHADER);
	ppShader.link();
	if (ppShader.getIsBad())
	{
		cerr << "Post-processing shader program is bad\n";
		throw runtime_error("Post-processing shader program is bad");
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

	// Bind uniform block
	GLuint blockIndex = glGetUniformBlockIndex(rtShader.id, "SceneBlock");
	glUniformBlockBinding(rtShader.id, blockIndex, 0);

	// Create uniform buffer object
	GLuint uniformBufferId;
	glGenBuffers(1, &uniformBufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferId);
	glBufferData(GL_UNIFORM_BUFFER, 16384, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Bind ubo
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uniformBufferId, 0, 16384);

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
	while (glGetError());
	glGetUniformIndices(rtShader.id, count, names, indices);
	cerr << "error: " << glGetError() << '\n';
	cerr << indices[0] << ", " << indices[1] << ", " << indices[2] << ", " << indices[3] << ", " << indices[4] << ", " << indices[5] << ", " << indices[6] << '\n';

	GLint offsets[count];
	while (glGetError());
	glGetActiveUniformsiv(rtShader.id, count, indices, GL_UNIFORM_OFFSET, offsets);
	cerr << "error: " << glGetError() << '\n';
	cerr << offsets[0] << ", " << offsets[1] << ", " << offsets[2] << ", " << offsets[3] << ", " << offsets[4] << ", " << offsets[5] << ", " << offsets[6] << '\n';

	glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferId);

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
}

Game::~Game()
{
	if (frameBufferId != -1)
	{
		glDeleteFramebuffers(1, &frameBufferId); 
	}
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
	vec3 moveInput(
		window.getKeyDown(GLFW_KEY_D)     - window.getKeyDown(GLFW_KEY_A),
		window.getKeyDown(GLFW_KEY_SPACE) - window.getKeyDown(GLFW_KEY_LEFT_CONTROL),
		window.getKeyDown(GLFW_KEY_S)     - window.getKeyDown(GLFW_KEY_W));
	if (moveInput != vec3(0) || window.getMouseDelta() != vec2(0))
	{
		eyeController.moveCamera(moveInput * 2.f / 60.f, window.getMouseDelta());
		rtShader.bind();
		rtShader.setUniform("eyeTrans", eye.getTrans());
	}
}

void Game::render()
{
	// Resize off-screen texture if necessary
	if (window.getDidSizeChangeThisFrame())
	{
		glBindTexture(GL_TEXTURE_2D, frameTextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, window.getSize().x, window.getSize().y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	// Render to off-screen buffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferId);
	rtShader.bind();
	rtShader.setUniform("time", gameTime.getTime());
	screenMesh.bind();
	screenMesh.draw();

	// Render to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, frameTextureId);
	ppShader.bind();
	screenMesh.bind();
	screenMesh.draw();
	window.display();
}
