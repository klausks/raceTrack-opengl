#version 460 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 inNormal;

out vec2 TexCoord;
out vec3 normal;
out vec3 fragmentPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec4 model_times_vertex = model * vec4(vPosition, 1.0);
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	TexCoord = texCoord;
	normal = mat3(transpose(inverse(model))) * inNormal;  
	fragmentPos = vec3(model_times_vertex);
}