#version 460
layout(location = 0) in vec3 in_color; 
 
layout(location = 0) out vec4 frag_color; 

void main() { 
	frag_color = vec4(in_color, 0.0);
};