#version 330 core

out vec4 FragColor;
in vec2 TexCoords;
in vec3 aNormal;
in vec3 aPos;

uniform sampler2D texture_diffuse1;
uniform vec4 object_color;
uniform int use_diffuse_map;

uniform vec3 light_pos;
uniform float ambient;
uniform float shininess;
uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

void main()
{    
    
   

    if(use_diffuse_map == 1)
    {
        FragColor = texture2D(texture_diffuse1, TexCoords);
    }
    else
    {
        FragColor = object_color;
    }
    


    
   
}