#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTex_cord;
layout(location = 2) in vec3 aNormal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

uniform mat4 u_proj_view_matrix;
uniform mat4 u_model_matrix;

out vec3 world_position;
out vec2 tex_cord;
out mat3 TBN;

void main()
{
    world_position = (u_model_matrix * vec4(aPos, 1.0) ).xyz;
    tex_cord = aTex_cord;

    mat4 normal_matrix = transpose(inverse(u_model_matrix));
    vec3 T = normalize(mat3(normal_matrix) * aTangent);
    vec3 N = normalize(mat3(normal_matrix) * aNormal);

    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);

    gl_Position = u_proj_view_matrix * u_model_matrix * vec4(aPos, 1.0);
}