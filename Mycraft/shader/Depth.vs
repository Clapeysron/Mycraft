#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
uniform mat4 lightSpaceMatrix;
uniform mat4 model;

void main() {
    TexCoord = aTexCoord;
    gl_Position = lightSpaceMatrix * model * vec4(aPos, 1.0f);
}
