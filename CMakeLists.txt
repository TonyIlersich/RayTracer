cmake_minimum_required(VERSION 3.10.2)

project(
	RayTracer
	VERSION 1.0
	DESCRIPTION "Ray Tracer"
	LANGUAGES CXX
)

include_directories(external)
include_directories(src)

add_executable(${PROJECT_NAME}
	src/surfaces/Cube.cpp
	src/surfaces/Cylinder.cpp
	src/surfaces/Disc.cpp
	src/surfaces/NhPlane.cpp
	src/surfaces/Plane.cpp
	src/surfaces/Quad.cpp
	src/surfaces/Sphere.cpp
	src/ComputeShader.cpp
	src/Eye.cpp
	src/EyeController.cpp
	src/Game.cpp
	src/Hit.cpp
	src/Image.cpp
	src/main.cpp
	src/Material.cpp
	src/MaterialFactory.cpp
	src/Mesh.cpp
	src/Random.cpp
	src/Ray.cpp
	src/Scene.cpp
	src/SceneFactory.cpp
	src/SceneObject.cpp
	src/ShaderProgram.cpp
	src/Timer.cpp
	src/Transform.cpp
	src/Window.cpp
)

add_subdirectory(external/glad)
add_subdirectory(external/glfw)

# TODO: might have to add -lGL -lGLU if opengl doesn't link properly
target_link_libraries(${PROJECT_NAME} glad glfw)# -ldl -lm -lpthread -lX11)
