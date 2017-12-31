#version 330 core
layout (location = 0) in vec3 aPos;
out vec2 SkyTexCoords;
out vec3 starPos;
uniform mat4 projection;
uniform mat4 view;
uniform float DayPos;
const float M_PI = 3.14159265;

float getBlockPos(vec3 aPos) {
    if (aPos.y == 1) {
        return max(abs(aPos.x),abs(aPos.z))/4;
    } else if (aPos.y == -1) {
        return (1-max(abs(aPos.x),abs(aPos.z))/4);
    } else {
        return (1-aPos.y)/4;
    }
}

void main()
{
    starPos = aPos;
    vec2 outCoord = vec2(DayPos, getBlockPos(aPos));
    SkyTexCoords = outCoord;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}
