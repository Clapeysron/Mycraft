#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
} fs_in;

struct Sunlight {
    vec3 lightDirection;
    vec3 ambient;
    vec3 lightambient;
};

uniform Sunlight sunlight;

void main()
{
    vec3 color = texture(texture_diffuse1, fs_in.TexCoord).rgb;
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-sunlight.lightDirection);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = sunlight.lightambient * diff;
    
    //float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 result;
    result = (sunlight.ambient + diffuse) * color;
    //result = (sunlight.ambient + (1.0f -shadow) * diffuse) * color;
    FragColor = vec4(result, 1.0f);
}
