#version 330 core

#define SHOW_ALL 0
#define SHOW_AMBIENT 1
#define SHOW_DIFFUSE 2
#define SHOW_SPECULAR 3

out vec4 FragColor;
in vec2 uv0;
in vec3 normal;
in vec3 position;


uniform sampler2D texture_diffuse1;
uniform vec4 object_color;
uniform int use_diffuse_map;
uniform mat4 view;
uniform vec3 light_pos;
uniform float ambient;
uniform float shininess;
uniform float k_ambient;
uniform float k_diffuse;
uniform float k_specular;

void main()
{    
    
   

    vec3 normal      = normalize(normal);
    vec3 light_dir   = normalize(light_pos-position);
    float diffuse    = max(0.0, dot(normal, light_dir));
    float specular   = 0.0;

    if(diffuse > 0.0 && k_specular > 0.0 && shininess > 0.0)
    {
        vec3 view_pos    = transpose(mat3(view))*(-view[3].xyz);
        vec3 view_dir    = normalize(view_pos-position);
        vec3 half_dir    = normalize(view_dir+light_dir);
        float sp         = max(dot(normal, half_dir), 0.0);

        if(sp > 0.0)
        {
            specular = pow(sp, shininess); 
        }
    }
    
    float intensity = (k_ambient*ambient+k_diffuse*diffuse+k_specular*specular); 

    if(use_diffuse_map == 1)
    {
        FragColor = texture2D(texture_diffuse1, uv0);
    }
    else
    {
        FragColor = object_color;
    }

    FragColor = vec4(FragColor.rgb*intensity, FragColor.a);

    


    
   
}

