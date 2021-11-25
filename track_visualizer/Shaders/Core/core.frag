#version 460 core

in vec2 TexCoord;

out vec4 color;

uniform sampler2D texture1;

void main(){
	vec4 tex1 = texture( texture1, vec2(TexCoord.x, 1 - TexCoord.y ));
	color = tex1;
}