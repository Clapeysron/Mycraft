#version 330 core
in vec2 TexCoord;
uniform sampler2D texture_pic;
void main()
{
    float alpha = texture(texture_pic, TexCoord).a;
    if (alpha <= 0.1f) {
        discard;
    }
    //gl_FragDepth = gl_FragCoord.z;
}
