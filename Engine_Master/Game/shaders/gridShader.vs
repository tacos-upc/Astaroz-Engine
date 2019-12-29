#version 330
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec3 gl_Color;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec3 vColor;

void main()
{
	gl_Position = proj*view*model*vec4(aPos, 1.0);
	vColor = gl_Color;
}
