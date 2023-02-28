#include "includes.h"
#include "utils.h"
#include "Eye.h"
#include "Cube.h"
#include "colors.h"
#include "HalfSpace.h"
#include "Image.h"
#include "Scene.h"
#include "Sphere.h"
using namespace std;
using namespace glm;

// #define RENDER_IMAGE
// #define SINGLE_RAY

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracer", nullptr, nullptr);
	if (window == nullptr)
	{
		cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (gladLoadGL())
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		return -1;
	}

	while(!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

/* OLD: Image ray tracer
int main()
{
	Scene scene;
	Cube cube;
	Sphere sphere;
	HalfSpace halfSpace;
	Material mirror({}, { white });
	Material glass({}, { 0.1f * white }, { 0.9f * white, 0.f, 1.2f });
	Material matte({}, { gray, 1.f });
	Material redLaser({ red, 0.2f * white }, { black }, { white });
	Material greenLaser({ green, 0.2f * white }, { black }, { white });
	Material blueLaser({ blue, 0.2f * white }, { black }, { white });
	// Material glow({ 0.4f * red, 0.1f * white });

	SceneObject* mirrorBall = new SceneObject();
	mirrorBall->setVolume(&sphere);
	mirrorBall->setMaterial(&mirror);
	// mirrorBall->applyTransform(scale(mat4(1), vec3(1.5)));
	mirrorBall->applyTransform(translate(mat4(1), vec3(-2.5,0,-7)));
	scene.root.addChild(mirrorBall);

	SceneObject* glassBall = new SceneObject();
	glassBall->setVolume(&sphere);
	glassBall->setMaterial(&glass);
	glassBall->applyTransform(translate(mat4(1), vec3(.2f,.5f,-5.5)));
	scene.root.addChild(glassBall);

	SceneObject* redLaserBall = new SceneObject();
	redLaserBall->setVolume(&sphere);
	redLaserBall->setMaterial(&redLaser);
	redLaserBall->applyTransform(scale(mat4(1), vec3(1.5)));
	redLaserBall->applyTransform(translate(mat4(1), vec3(2,5,-8)));
	scene.root.addChild(redLaserBall);

	SceneObject* greenLaserBall = new SceneObject();
	greenLaserBall->setVolume(&sphere);
	greenLaserBall->setMaterial(&greenLaser);
	greenLaserBall->applyTransform(scale(mat4(1), vec3(1.5)));
	greenLaserBall->applyTransform(translate(mat4(1), vec3(-2,5,-8)));
	scene.root.addChild(greenLaserBall);

	SceneObject* blueLaserBall = new SceneObject();
	blueLaserBall->setVolume(&sphere);
	blueLaserBall->setMaterial(&blueLaser);
	blueLaserBall->applyTransform(scale(mat4(1), vec3(1.5)));
	blueLaserBall->applyTransform(translate(mat4(1), vec3(0,4,-9)));
	scene.root.addChild(blueLaserBall);

	// SceneObject* glowCube = new SceneObject();
	// glowCube->setVolume(&cube);
	// glowCube->setMaterial(&glow);
	// glowCube->applyTransform(rotate(mat4(1), tau * 0.125f, vec3(0,1,0)));
	// glowCube->applyTransform(translate(mat4(1), vec3(2,0,-9)));
	// scene.root.addChild(glowCube);

	// SceneObject* glowCube2 = new SceneObject();
	// glowCube2->setVolume(&cube);
	// glowCube2->setMaterial(&glow);
	// glowCube2->applyTransform(rotate(mat4(1), tau * -1.f / 16.f, vec3(0,1,0)));
	// glowCube2->applyTransform(translate(mat4(1), vec3(2.5,-1.1,-9)));
	// scene.root.addChild(glowCube2);

	// SceneObject* ground = new SceneObject();
	// ground->setVolume(&sphere);
	// ground->setMaterial(&matte);
	// ground->applyTransform(scale(translate(mat4(1), vec3(0,-101,0)), vec3(100)));
	// scene.root.addChild(ground);

	SceneObject* ground = new SceneObject();
	ground->setVolume(&halfSpace);
	ground->setMaterial(&matte);
	ground->applyTransform(translate(mat4(1), vec3(0,-1,0)));
	scene.root.addChild(ground);

	Eye eye(vec3(0,1,0), vec3(0,0,-1), vec3(0,1,0), vec2(radians(60.f)));

#ifdef RENDER_IMAGE
	Image image(1920, 1080);
	constexpr int super = 2;
	const vec2 pixelSize = 2.f / vec2(image.getWidth(), image.getHeight());
	const vec2 sampleSize = pixelSize / float(super);
	auto start = chrono::system_clock::now();
	for (int x = 0; x < image.getWidth(); x++)
	{
		const float progress = x / (image.getWidth() - 1.f);
		const int progressBarWidth = 50;
		const int ticks = progress * progressBarWidth;
		cerr << "\r[" << string(ticks, '=') << string(progressBarWidth - ticks, ' ') << "] " << setw(3) << floor(progress * 100.f) << '%';
		for (int y = 0; y < image.getHeight(); y++)
		{
			// image(x, y) = scene.getRayColor(
			// 	eye.getRay(
			// 		2.f * x / (image.getWidth() - 1) - 1.f,
			// 		-(2.f * y / (image.getHeight() - 1) - 1.f)
			// 	)
			// );

			for (int sx = 0; sx < super; sx++)
			{
				for (int sy = 0; sy < super; sy++)
				{
					image(x, y) += scene.getRayColor(
						eye.getRay(
							2.f * x / (image.getWidth() - 1) - 1.f + sampleSize.x * sx,
							-(2.f * y / (image.getHeight() - 1) - 1.f + sampleSize.y * sy)
						)
					);
				}
			}
			image(x, y) /= super * super;
		}
	}
	cerr << '\n';
	auto end = chrono::system_clock::now();
	cerr << "Rendered in " << (end - start).count() / 1000'000'000 << "s\n";
	image.saveBmp("output.bmp");
#endif

#ifdef SINGLE_RAY
	cerr << glass.refraction.index << '\n';
	cerr << scene.getRayColor(eye.getRay(0.25f, 0.f)) << '\n';
#endif

	return 0;
}
*/