#version 330 core
layout (location = 0) in vec3 aPos;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;
out vec4 FragPos;

void main()
{
    gl_Position = projection * model * view * vec4(aPos, 1.0f);
    FragPos = model * vec4(aPos, 1.0f);
}
