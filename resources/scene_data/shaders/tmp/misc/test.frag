#version 330 core
layout(location = 0) out vec4 out_albedo;

uniform sampler2D u_albedoMap;

in vec3 position;
in vec2 tex_cord;

void main()
{
    out_albedo = texture(u_albedoMap, tex_cord);
}