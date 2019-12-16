#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 aNormal;
out vec3 aPos;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main()
{
	gl_Position = proj*view*model*vec4(aPos, 1.0);
	aPos= (model*vec4(vertex_normal, 1.0)).xyz;
	TexCoords = aTexCoords;
	aNormal      = (model*vec4(vertex_normal, 0.0)).xyz;
	
}