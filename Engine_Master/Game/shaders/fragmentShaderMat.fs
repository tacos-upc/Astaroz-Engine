#version 330 core

struct Material
{
    sampler2D diffuse_texture;
    vec4      diffuse_color;

    sampler2D specular_texture;
    vec3      specular_color;
    float     shininess;

    sampler2D occlusion_texture;

    sampler2D emissive_texture;
    vec3      emissive_color;

    float     k_ambient;
    float     k_diffuse;
    float     k_specular;
};


in vec2 uv0;
in vec3 normal;
in vec3 position;
in mat4 aView;

out vec4 FragColor;

uniform Material material;
uniform vec3 light_pos;

vec4 get_diffuse_color(Material material, vec2 uv0)
{
	return texture(material.diffuse_texture, uv0)*material.diffuse_color;
}

vec3 get_specular_color(Material material, vec2 uv0)
{
	return texture(material.specular_texture, uv0).rgb*material.specular_color;
}

vec3 get_occlusion_color(Material material, vec2 uv0)
{
	return texture(material.occlusion_texture, uv0).rgb;
}

vec3 get_emissive_color(Material material, vec2 uv0)
{
	return texture(material.emissive_texture, uv0).rgb*material.emissive_color;
}


float lambert(vec3 direction, vec3 normals)
{
	return max(dot(normals, direction), 0.0);
}

float specular_phong(vec3 direction, vec3 pos, vec3 normals, vec3 viewPos, float shininess)
{
	vec3 viewDir = normalize(viewPos-pos);
	vec3 halfDir = normalize(viewDir+direction);
	float specular= 0.0;

	float sp= max(dot(normals, halfDir), 0.0);
	if(sp > 0.0)
        {
            specular = pow(sp, shininess); 
        }
	return specular;
}

vec3 directional_phong(vec3 normal, vec3 viewPos, vec3 light_pos, vec4 diffuse_color, vec3 specular_color, float shininess, Material material)
{
	vec3 light_dir = normalize(light_pos-position);
	float diffuse = lambert(light_dir , normal);
	float specular = specular_phong(light_dir, position, normal, viewPos, shininess);

	vec3 color =(diffuse_color.rgb * diffuse * material.k_diffuse + specular_color.rgb * specular * material.k_specular); 
	return color;
}

void main()
{
	vec3 normal = normalize(normal);
	vec3 viewPos = transpose(mat3(aView))*(-aView[3].xyz);

	vec4 diffuse_color = get_diffuse_color(material, uv0);
	vec3 specular_color = get_specular_color(material, uv0);
	vec3 emissive_color = get_emissive_color(material, uv0);
	vec3 occlusion_color= get_occlusion_color(material, uv0);

	vec3 color = directional_phong(normal, viewPos, light_pos, diffuse_color, specular_color, material.shininess, material);
	

	color += emissive_color + diffuse_color.rgb * occlusion_color * material.k_ambient; 
	
	
	FragColor = vec4(color, diffuse_color.a);


}
	



