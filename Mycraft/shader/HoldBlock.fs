#version 330 core
out vec4 FragColor;
uniform sampler2D texture_pic;
in vec2 TexCoord;

void main()
{
    FragColor = texture(texture_pic, TexCoord).rgba;
}

