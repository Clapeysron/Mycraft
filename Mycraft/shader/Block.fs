#version 330 core
out vec4 FragColor;
uniform sampler2D texture_pic;
uniform sampler2D shadowMap;
uniform sampler2D skybox;
uniform float DayPos;
uniform float starIntensity;
uniform float broken_texture_x;
uniform float noFogRadius;
uniform bool eye_in_water;
in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoord;
    vec4 FragPosLightSpace;
    float shadow;
    float brightness;
    vec4 ViewPos;
} fs_in;

struct Sunlight {
    vec3 lightDirection;
    vec3 ambient;
    vec3 lightambient;
};

uniform Sunlight sunlight;
uniform vec3 chosen_block_pos;
uniform bool isDaylight;

const float fogDensity = 0.05;

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
    float bias = max(0.0008 * (1.0 - dot(normal, lightDir)), 0.0008);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
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
    vec3 diffuse = sunlight.lightambient * diff;
    float isChosen = isChosen(fs_in.FragPos);
    
    //Chosen and Break
    if (isChosen!= 1.0f && broken_texture_x!= 0.0f) {
        float real_broken_texture_x = broken_texture_x + fs_in.TexCoord.x - floor(fs_in.TexCoord.x*10)/10;
        float real_broken_texture_y = fs_in.TexCoord.y - floor(fs_in.TexCoord.y*10)/10 + 0.9;
        vec4 broken_texture = texture(texture_pic, vec2(real_broken_texture_x, real_broken_texture_y));
        if (broken_texture.a>0.05f) {
            color = (broken_texture.r+0.1)*2 * color;
        }
    }
    
    //below water
    if (eye_in_water) {
        float real_water_texture_x = fs_in.TexCoord.x - floor(fs_in.TexCoord.x*10)/10 + 0.9;
        float real_water_texture_y = fs_in.TexCoord.y - floor(fs_in.TexCoord.y*10)/10 + 0.1;
        vec4 water_texture = texture(texture_pic, vec2(real_water_texture_x, real_water_texture_y));
        if (fs_in.TexCoord.x<0.9 || fs_in.TexCoord.y<0.1 || fs_in.TexCoord.y>0.3) {
            color = mix(water_texture.rgb, color, 0.8);
        }
    }
    
    float shadow;
    // With Shadow mapping
    if (isDaylight) {
        shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    } else {
        shadow = 1.0f;
    }
    vec3 result;
    if (alpha == 1.0f) {
        // Without Shadow mapping
        //result = (sunlight.ambient + diffuse) * isChosen * color;
        // With Shadow mapping
        result = (sunlight.ambient + (1.1f - shadow) * diffuse) * isChosen * color;
    } else if (alpha == 0.0f) {
        discard;
    } else {
        // Without Shadow mapping
        //result = fs_in.brightness * fs_in.shadow * isChosen * color;
        //result = isChosen * color;
        // With Shadow mapping
        result = fs_in.brightness * fs_in.shadow * (sunlight.ambient +(1.1f-shadow)*diffuse) * isChosen * color;
    }
    
    //fog
    
    float dist = (length(fs_in.ViewPos)<noFogRadius) ? 0 : length(fs_in.ViewPos)-noFogRadius;
    float fogFactor = 1.0/exp((dist*fogDensity)*(dist*fogDensity));
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec2 SkyTexCoords = vec2(DayPos, 0.5);
    vec4 fogColor = (1-starIntensity) * texture(skybox, SkyTexCoords) + starIntensity * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    FragColor = mix( fogColor, vec4(result, alpha), fogFactor);
}
