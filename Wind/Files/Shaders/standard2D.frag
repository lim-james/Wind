#version 330 core

out vec4 color;

in VS_OUT {
	vec2 texCoord;
	vec4 color;
} vs_out;

uniform sampler2D tex;

void main() {
	color = texture(tex, vs_out.texCoord) * vs_out.color;	
}
