#version 330 core

uniform vec3 u_albedo;
uniform sampler2D u_albedoMap;
uniform int u_hasAlbedoMap;

uniform float u_ambientStrengt;
uniform vec3 u_lightDir;

out vec4 color_out;

in vec3 position;
in vec3 normal;
in vec2 tex_cord;

vec3 GetAlbedo()
{
    if(u_hasAlbedoMap > 0)
    {
        return texture(u_albedoMap, tex_cord).rgb;
    }

    return u_albedo;
}

void main()
{
    vec3 albedo = GetAlbedo();
    vec3 norm   = normalize(normal);

    float diffuse = max(dot(norm, -u_lightDir), 0.0);

    vec3 result = albedo * (u_ambientStrengt + diffuse);
    color_out = vec4(result, 1.0);
}