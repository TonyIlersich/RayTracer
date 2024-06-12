R""(
#version 330 core

layout (location = 0) in vec2 screenPos;

out vec2 uv;
	
void main()
{
    uv = (screenPos + 1.0) / 2.0;
    gl_Position = vec4(screenPos, 0.0, 1.0);
}
)""