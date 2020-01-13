#version 330 core

struct Material
{
    sampler2D diffuse_texture;
    vec4 diffuse_color;

    sampler2D specular_texture;
    vec3 specular_color;
    float shininess;

    sampler2D occlusion_texture;

    sampler2D emissive_texture;
    vec3 emissive_color;

    float k_ambient;
    float k_diffuse;
    float k_specular;
};


in vec2 uv0;
in vec3 normal;
in vec3 position;
in mat4 aView;

out vec4 FragColor;

uniform Material material;
uniform vec3 light_pos;


void main()
{

	vec3 norm = normalize(normal);
	vec3 viewPos = transpose(mat3(aView))*(-aView[3].xyz);

	vec4 diffuse_color = texture(material.diffuse_texture, uv0)*material.diffuse_color;
	vec3 specular_color = texture(material.specular_texture, uv0).rgb*material.specular_color;
	vec3 emissive_color = texture(material.emissive_texture, uv0).rgb*material.emissive_color;
	vec3 occlusion_color= texture(material.occlusion_texture, uv0).rgb;

	float specular= 0.0;

	vec3 light_dir = normalize(light_pos-position);
	float diffuse = max(0.0, dot(norm, light_dir));

	if(diffuse > 0.0 && material.shininess > 0.0){

		vec3 viewDir = normalize(viewPos-position);
		vec3 reflect_dir = normalize(reflect(-light_dir, norm));
        	float sp = max(dot(viewDir, reflect_dir), 0.0);
	

		if(sp > 0.0)
        	{
            		specular = pow(sp, material.shininess); 
        	}
	}

	vec3 color =diffuse_color.rgb * diffuse * material.k_diffuse + specular_color * specular * material.k_specular +emissive_color + diffuse_color.rgb * occlusion_color * material.k_ambient; 
	
	
	FragColor = vec4(color, diffuse_color.a);
}

