#version 460 core

layout (location = 0) in vec3 vPosition;

out vec3 vColor;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(vPosition, 1.0);
	vColor = vec3(1.0 - vPosition.z/255.0, 0.0, vPosition.z/255.0);
}