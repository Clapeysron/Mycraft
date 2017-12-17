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
    vec3 lightDirection;
    vec3 ambient;
};

uniform Sunlight sunlight;
uniform vec3 chosen_block_pos;

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
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-sunlight.lightDirection);
    float bias = max(0.0005 * (1.0 - dot(normal, lightDir)), 0.0005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    if(projCoords.z > 1.0)
        shadow = 0.0;
    //float shadow = (currentDepth - 0.0005) > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

float isChosen(vec3 FragPos) {
    if ( FragPos.x < chosen_block_pos.x || FragPos.x > chosen_block_pos.x + 1 ||
         FragPos.y < chosen_block_pos.y || FragPos.y > chosen_block_pos.y + 1 ||
         FragPos.z < chosen_block_pos.z || FragPos.z > chosen_block_pos.z + 1 ) return 1.0f;
    return 1.4f;
}

void main()
{
    vec3 color = texture(texture_pic, fs_in.TexCoord).rgb;
    float alpha = texture(texture_pic, fs_in.TexCoord).a;
    vec3 norm = normalize(fs_in.Normal);
    vec3 lightDir = normalize(-sunlight.lightDirection);
    float diff = max(dot(lightDir, norm), 0.0);
    vec3 diffuse = sunlight.ambient * diff * 3;
    float isChosen = isChosen(fs_in.FragPos);
    
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 result;
    if (alpha == 1.0f) {
        //result = (sunlight.ambient + diffuse) * color;
        result = (sunlight.ambient + (1.0f - shadow) * diffuse) * isChosen * color;
    } else {
        //result = color;
        result = (1.0f - shadow) * isChosen * color;
    }
    FragColor = vec4(result, alpha);
}
