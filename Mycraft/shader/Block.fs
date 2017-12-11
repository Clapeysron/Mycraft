#version 330 core
out vec4 FragColor;
uniform sampler2D texture_pic;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Sunlight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
};
uniform Sunlight sunlight;

void main()
{
    vec3 color = texture(texture_pic, TexCoord).rgb;
    float alpha = texture(texture_pic, TexCoord).a;
    vec3 ambient = sunlight.ambient * color;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-sunlight.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = sunlight.diffuse * diff * color;
    vec3 result;
    if (alpha == 1.0f) {
        result = ambient + diffuse;
    } else {
        result = color;
    }
    FragColor = vec4(result,alpha);
}
