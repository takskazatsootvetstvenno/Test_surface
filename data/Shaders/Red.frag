#version 460

layout(location = 0) in vec3 Normal; 
layout(location = 0) out vec4 frag_color; 

void main() { 
	const vec3 lightDir = normalize(vec3(1.f, 1.f, 1.f));
	const vec3 norm = normalize(Normal);
	const float diff = max(dot(norm, lightDir), 0.0);
	frag_color = (diff + 0.2f) * vec4(0.9, 0.8, 0.8, 0.0);
};