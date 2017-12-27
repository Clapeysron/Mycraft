#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D Sun_Moon_texture;
uniform float dayTime;
const float M_PI = 3.14159265;
void main()
{
    float dayIntensity = sin((dayTime-8)*M_PI/24)*0.2;
    FragColor = (0.9 + dayIntensity) * texture(Sun_Moon_texture, TexCoord);
    FragColor.r = FragColor.r * (1.2 - dayIntensity);
    
}
