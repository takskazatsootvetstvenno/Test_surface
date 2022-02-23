#version 460

layout(location = 0) in vec3 Normal; 
layout(location = 0) out vec4 frag_color; 

 layout(std140, binding = 2) uniform LightInfo
{
    vec3 LightPos;
	vec3 Color;
};

void main() { 
	const vec3 lightDir = normalize(LightPos);
	const vec3 norm = normalize(Normal);
	const float diff = max(dot(norm, lightDir), 0.0);
	frag_color = (diff + 0.2f) * vec4(Color, 0.0);
};