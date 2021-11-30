#version 460 core

in vec2 TexCoord;
in vec3 normal;
in vec3 fragmentPos;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;  

void main(){
	vec4 textureColor = texture( texture1, vec2(TexCoord.x, 1 - TexCoord.y ));
	
    // ambient lighting
    vec3 ambient = material.ambient * light.ambient;
    
    // diffuse lighting
    vec3 norm = normalize(normal);
    vec3 lightDir = lightPos - fragmentPos;
	vec3 lightDirNormalized = normalize(lightDir);
    float lightDist = length(lightDir);
    float attenC1 = 0.01f;
    float attenC2 = 0.005f;
    float attenC3 = 0.002f;
    float attenFactor = min( 1 / (attenC1 + attenC2 * lightDist + attenC3 * pow(lightDist, 2)), 1);
	float diff = max(dot(norm, lightDirNormalized), 0.0);
	vec3 diffuse = attenFactor * lightColor * (diff * material.diffuse) * light.diffuse;

    // specular lighting
    vec3 viewDir = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDirNormalized, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = lightColor * (spec * material.specular) * light.specular;
		
	vec3 final_light = ambient + diffuse + specular;

	fragColor = vec4(final_light, 1.0) * textureColor;
}