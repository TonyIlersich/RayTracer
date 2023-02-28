cmake_minimum_required(VERSION 3.10.2)

project(
	RayTracer
	VERSION 1.0
	DESCRIPTION "Ray Tracer"
	LANGUAGES CXX
)

include_directories(external)
include_directories(src)

# add_library(glad include/glad/glad.c)

add_executable(${PROJECT_NAME}
	src/main.cpp
	src/Cube.cpp
	src/Cylinder.cpp
	src/Eye.cpp
	src/HalfSpace.cpp
	src/Hit.cpp
	src/Image.cpp
	src/Material.cpp
	src/Plane.cpp
	src/Quad.cpp
	src/Random.cpp
	src/Ray.cpp
	src/Scene.cpp
	src/SceneObject.cpp
	src/Sphere.cpp
)

add_subdirectory(external/glad)

# TODO: might have to add -lGL -lGLU if opengl doesn't link properly
target_link_libraries(${PROJECT_NAME} glad libglfw3.a -ldl -lm -lpthread -lX11)
