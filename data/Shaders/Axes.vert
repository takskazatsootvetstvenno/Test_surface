 #version 460
 layout(location = 0) in vec3 vertex_position; 
 layout(location = 1) in vec3 vertex_color; 

 layout(location = 0) out vec3 out_color; 

 layout(std140) uniform GlobalMatrices
{
    mat4 ProjMatrix;
    mat4 ViewMatrix;
    mat4 ModelMatrix;
};

 void main() { 
	out_color = vertex_color;
    gl_Position = ProjMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.0);
 } ;