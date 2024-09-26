#version 330 core
layout (location = 0) in vec3 aPos;
layout(location = 1) in vec2 atex_cord;

out vec2 tex_cord;

uniform mat4 u_proj;
uniform mat4 u_view;

void main()
{
    tex_cord = atex_cord;
    gl_Position = u_proj * u_view * vec4(aPos, 1.0);
}