 #version 460
 layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 ProjMatrix;
    mat4 ViewMatrix;
    mat4 ModelMatrix;
};

layout(std430, binding = 1) readonly buffer SSBO
{
    readonly vec3 in_point[][3];
};
layout(std430, binding = 2) readonly buffer Norm_SSBO
{
    readonly vec3 in_norm[][3];
};

layout(location = 0) out vec3 Normal; 

 void main() { 

    const vec3 point = in_point[gl_VertexID/3][gl_VertexID%3].xyz;
    const vec3 norm = in_norm[gl_VertexID/3][gl_VertexID%3].xyz;
    Normal = mat3(transpose(inverse(ModelMatrix))) * norm;
    gl_Position = ProjMatrix * ViewMatrix * ModelMatrix * vec4(point.xyz, 1.f);
 }