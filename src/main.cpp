#include "includes.h"
#include "colors.h"
#include "Cube.h"
#include "Eye.h"
#include "Game.h"
#include "Image.h"
#include "Scene.h"
#include "SceneFactory.h"
#include "Sphere.h"
#include "Timer.h"
#include "utils.h"
#include "Window.h"
using namespace std;
using namespace glm;

// /* Render to window
int main()
{
	Game().run();
	return 0;
}
/* */



/* OLD: Image ray tracer
#define SHOW_PROGRESS_BAR
#define RENDER_IMAGE
// #define TEST_RAY
// #define TEST_EDGE_RAYS

int main()
{
	Scene* scene = SceneFactory::createSampleScene();
	Eye eye(radians(60.f), translate(identityTrans, vec3(0, 2, 2)));

#ifdef RENDER_IMAGE
	Image image(1920, 1080);
	constexpr int skip = 1;
	constexpr int offset = 0;
	constexpr int super = 2;
	const int imageMajorAxis = glm::max(image.getWidth(), image.getHeight());
	const int imageMinorAxis = glm::min(image.getWidth(), image.getHeight());
	auto start = chrono::system_clock::now();
	for (int x = 0; x < image.getWidth(); x++)
	{
#ifdef SHOW_PROGRESS_BAR
		if (x % 10 == 0)
		{
			const float progress = x / (image.getWidth() - 1.f);
			const int progressBarWidth = 50;
			const int ticks = int(progress * progressBarWidth);
			cerr << "\r[" << setw(ticks) << setfill('=') << "" << setw(progressBarWidth - ticks) << setfill(' ') << "" << "] "
				<< setw(3) << floor(progress * 100.f) << '%';
		}
#endif
		for (int y = offset; y < image.getHeight(); y += skip)
		{
			for (int sx = 0; sx < super; sx++)
			{
				for (int sy = 0; sy < super; sy++)
				{
					image(x, y) += scene->getRayColor(eye.getRay(
						2.f * ((x + (sx + 0.5f) / super) - image.getWidth()  / 2) / imageMajorAxis,
						-2.f * ((y + (sy + 0.5f) / super) - image.getHeight() / 2) / imageMajorAxis));
				}
			}
			image(x, y) /= super * super;
		}
	}
#ifdef SHOW_PROGRESS_BAR
	cerr << '\n';
#endif
	auto end = chrono::system_clock::now();
	cerr << "Rendered in " << chrono::duration_cast<chrono::seconds>(end - start).count() << "s\n";
	image.saveBmp("output.bmp");
#endif

#ifdef TEST_RAY
	Ray ray = eye.getRay( 0.f,  0.5f);
	cerr << "Ray:    " << ray << '\n';
	cerr << " color: " << scene->getRayColor(ray) << '\n';
#endif

#ifdef TEST_EDGE_RAYS
	cerr << "Center:      " << eye.getRay( 0.f,  0.f) << '\n';
	cerr << " color:      " << scene->getRayColor(eye.getRay( 0.f,  0.f)) << '\n';
	cerr << "TopLeft:     " << eye.getRay(-1.f,  1.f) << '\n';
	cerr << " color:      " << scene->getRayColor(eye.getRay(-1.f,  1.f)) << '\n';
	cerr << "TopRight:    " << eye.getRay( 1.f,  1.f) << '\n';
	cerr << " color:      " << scene->getRayColor(eye.getRay( 1.f,  1.f)) << '\n';
	cerr << "BottomLeft:  " << eye.getRay(-1.f, -1.f) << '\n';
	cerr << " color:      " << scene->getRayColor(eye.getRay(-1.f, -1.f)) << '\n';
	cerr << "BottomRight: " << eye.getRay( 1.f, -1.f) << '\n';
	cerr << " color:      " << scene->getRayColor(eye.getRay( 1.f, -1.f)) << '\n';
#endif

	delete scene;
	return 0;
}
/* */