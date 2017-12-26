#version 330 core
layout (location = 0) in vec2 aPos;
out vec2 TexCoords;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0, 1.0);
    gl_Position = pos.xyww;
}
