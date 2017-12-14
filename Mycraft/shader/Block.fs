#version 330 core
out vec4 FragColor;
uniform sampler2D texture_pic;
uniform sampler2D shadowMap;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
} fs_in;

struct Sunlight {
    vec3 lightPos;
    vec3 ambient;
};

uniform Sunlight sunlight;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

void main()
{
    vec3 color = texture(texture_pic, fs_in.TexCoord).rgb;
    float alpha = texture(texture_pic, fs_in.TexCoord).a;
    vec3 norm = normalize(fs_in.Normal);
    
    vec3 ambient = sunlight.ambient * color;
    
    vec3 lightDir = normalize(sunlight.lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = ambient * diff;
    
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 result;
    if (alpha == 1.0f) {
        result = (ambient + (1.0f - shadow) * (diffuse)) * color;
    } else {
        result = (1.0f - shadow) * color;
    }
    FragColor = vec4(result, alpha);
}
