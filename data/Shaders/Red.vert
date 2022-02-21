 #version 460
 layout(std140, binding = 0) uniform GlobalMatrices
{
    mat4 ProjMatrix;
    mat4 ViewMatrix;
    mat4 ModelMatrix;
};

layout(std430, binding = 1) readonly buffer SSBO
{
    readonly vec4 data_SSBO[];
};

 void main() { 
    gl_Position = ProjMatrix * ViewMatrix * ModelMatrix * vec4(data_SSBO[gl_VertexID].xyz, 1.f);
 }