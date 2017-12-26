#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D Sun_Moon_texture;
void main()
{
    FragColor = texture(Sun_Moon_texture, TexCoord);
}
