R""(
#version 330 core

layout (location = 0) in vec2 screenPos;

out vec3 rayOrigin;
out vec3 rayDirection;

uniform float fovy;
uniform float aspect;
uniform mat4 eyeTrans;

void main()
{
	rayOrigin = vec3(eyeTrans * vec4(0.0, 0.0, 0.0, 1.0));
	float scale = tan(fovy * 0.5);
	rayDirection = vec3(eyeTrans * vec4(scale * screenPos.x * aspect, scale * screenPos.y, -1.0, 0.0));
	gl_Position = vec4(screenPos, 0.0, 1.0);
}
)""