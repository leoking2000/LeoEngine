#version 330 core
layout(location = 0) in vec3  aPos;
layout(location = 1) in vec2  aTex_cord;
layout(location = 2) in vec3  aNormal;
layout(location = 3) in vec3  aTangent;
layout(location = 4) in vec3  aBitangent;
layout(location = 5) in mat4  aInstanceMatrix;


uniform mat4 u_proj_view_matrix;
uniform mat4 u_model_matrix;
uniform mat3 u_normal_matrix;

uniform int u_hasInstanceArray;

out vec3 position;
out vec3 normal;
out vec2 tex_cord;

void main()
{
    if(u_hasInstanceArray == 0)
    {
        gl_Position = u_proj_view_matrix * u_model_matrix * vec4(aPos, 1.0);

        position = vec3(u_model_matrix * vec4(aPos, 1.0));
        normal   = u_normal_matrix * aNormal;
        tex_cord = aTex_cord;
    }
    else
    {
        gl_Position = u_proj_view_matrix * u_model_matrix * aInstanceMatrix * vec4(aPos, 1.0);

        position = vec3(u_model_matrix * aInstanceMatrix * vec4(aPos, 1.0));
        normal   = mat3(transpose(inverse(u_model_matrix * aInstanceMatrix))) * aNormal;
        tex_cord = aTex_cord;
    }

}
