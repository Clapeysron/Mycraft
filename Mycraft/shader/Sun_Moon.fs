#version 330 core
out vec4 FragColor;
in vec2 TexCoord;
uniform sampler2D Sun_Moon_texture;
uniform float dayTime;
uniform bool isSun;
const float M_PI = 3.14159265;
void main()
{
    vec4 outColor = texture(Sun_Moon_texture, TexCoord);
    if (isSun) {
        outColor = outColor*1.1;
        vec2 redSunTexCoord = vec2(TexCoord.x+0.1, TexCoord.y);
        vec4 redSun = texture(Sun_Moon_texture, redSunTexCoord);
        if (dayTime >=5.5 && dayTime <= 6.5) {
            float dayIntensity = sin((6.5-dayTime)*M_PI);
            FragColor = dayIntensity*redSun + (1-dayIntensity)*outColor;
        } else if (dayTime<6 || dayTime>18) {
            FragColor = redSun;
        } else if (dayTime >= 17 && dayTime <= 19) {
            float dayIntensity = sin((dayTime-17)*M_PI/2);
            FragColor = dayIntensity*redSun + (1-dayIntensity)*outColor;
        } else {
            FragColor = outColor;
        }
    } else {
        FragColor = outColor;
    }
}
