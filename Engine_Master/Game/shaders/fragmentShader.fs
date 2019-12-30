#version 330 core

out vec4 FragColor;
in vec2 TexCoords;


uniform sampler2D texture_diffuse1;
uniform vec4 object_color;
uniform int use_diffuse_map;



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