
#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

out vec2 uv0;
out vec3 normal;
out vec3 position;

void main()
{
    position    = (model*vec4(aPos, 1.0)).xyz;
    normal      = (model*vec4(aNormal, 0.0)).xyz;
    gl_Position = proj*view*vec4(position, 1.0);
    uv0 = aTexCoords;
}