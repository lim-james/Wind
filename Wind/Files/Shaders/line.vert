#version 330 core

layout(location = 0) in vec4 iPosition;
layout(location = 1) in vec4 iTint;

out VS_OUT {
	vec2 texCoord;
	vec4 color; 
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * vec4(iPosition, 0.f, 1.f);
	vs_out.texCoord = vec2(0.f);
	vs_out.color = iTint;
}