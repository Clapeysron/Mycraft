#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} vs_out;

void main()
{
    vs_out.Normal = aNormal;
    vs_out.FragPos = aPos;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0f);
    vs_out.TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    gl_Position = projection * view * vec4(aPos, 1.0f);
}
