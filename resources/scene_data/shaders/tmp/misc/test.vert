#version 330 core
layout(location = 0) in vec3  apos;
layout(location = 1) in vec2  atex_cord;
layout(location = 2) in vec3  anormal;
layout(location = 3) in vec3  aTangent;
layout(location = 4) in vec3  aBitangent;
layout(location = 5) in mat4  ainstanceMatrix;

uniform mat4 u_proj_matrix;
uniform mat4 u_view_matrix;

out vec3 position;
out vec2 tex_cord;

void main()
{
    position  = (ainstanceMatrix * vec4(apos, 1.0) ).xyz;
    tex_cord = atex_cord;

    gl_Position = u_proj_matrix * u_view_matrix * ainstanceMatrix * vec4(apos, 1.0);
}