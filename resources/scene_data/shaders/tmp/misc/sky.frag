#version 330 core

#define PI 3.14159

in vec2 tex_cord;
uniform sampler2D u_skybox;

out vec4 out_color;

void main()
{
    out_color = texture(u_skybox, tex_cord);
}