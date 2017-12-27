#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoord = aTexCoord;
    vec4 pos = projection * view * model * vec4(aPos, 0.0, 1.0);
    pos.z = pos.w;
    pos.z -= 0.01;
    gl_Position = pos;
}
