#version 330 core
layout(location = 0) in vec3  aPos;
layout(location = 1) in vec2  aTex_cord;
layout(location = 2) in vec3  aNormal;
layout(location = 3) in vec3  aTangent;
layout(location = 4) in vec3  aBitangent;
layout(location = 5) in mat4  aInstanceMatrix;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

out vec3 position;
out vec2 tex_cord;
out mat3 TBN;

void main()
{
    position  = (aInstanceMatrix * vec4(aPos, 1.0) ).xyz;
    tex_cord = aTex_cord;

    mat4 normal_matrix = transpose(inverse(aInstanceMatrix));

    vec3 T = normalize(mat3(normal_matrix) * aTangent);
    vec3 B = normalize(mat3(normal_matrix) * aBitangent);
    vec3 N = normalize(mat3(normal_matrix) * aNormal);
    TBN = mat3(T, B, N);

    gl_Position = u_proj_matrix * u_view_matrix * aInstanceMatrix * vec4(aPos, 1.0);
}