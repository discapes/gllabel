#version 330 core
layout(location = 0) in vec2 vCoord;
layout(location = 1) in vec2 vUV;

out vec2 oUV;

void main()
{
	gl_Position = vec4(vCoord, 0, 1);
	oUV = vUV;
}
