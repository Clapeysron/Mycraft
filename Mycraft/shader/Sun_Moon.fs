#version 330 core
out vec4 FragColor;
in vec3 TexCoords;
uniform sampler2D Sun_Moon_texture;
void main()
{
    FragColor = texture(Sun_Moon_texture, TexCoords);
}
