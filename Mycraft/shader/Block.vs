#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 Normal;

void main()
{
    gl_Position = projection * model * view * vec4(aPos, 1.0f);
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0f));
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}
