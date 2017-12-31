#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float shadow;
layout (location = 4) in float brightness;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    float shadow;
    float brightness;
    vec4 ViewPos;
} vs_out;

void main()
{
    vs_out.Normal = aNormal;
    vs_out.FragPos = aPos;
    vs_out.TexCoord = aTexCoord;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(aPos, 1.0f);
    vs_out.shadow = shadow;
    vs_out.brightness = brightness;
    vs_out.ViewPos = view * vec4(aPos, 1.0f);
    gl_Position = projection * vs_out.ViewPos;
}
