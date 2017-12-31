#version 330 core
out vec4 FragColor;
in vec2 SkyTexCoords;
in vec3 starPos;
uniform sampler2D skybox;
uniform samplerCube star;
uniform float starIntensity;
void main()
{
    FragColor = (1-starIntensity) * texture(skybox, SkyTexCoords) + starIntensity * texture(star, starPos);
}
