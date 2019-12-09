#version 330 core

out vec4 FragColor;
in vec2 TexCoords;

uniform vec4 object_color;
uniform sampler2D texture_diffuse1;


void main()
{    
    
    FragColor = object_color;
   
    
}