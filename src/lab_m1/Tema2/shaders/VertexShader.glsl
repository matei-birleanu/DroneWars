#version 330

layout(location=0) in vec3 v_position;
layout(location=1) in vec3 v_normal;
layout(location=2) in vec2 text_coord;
layout(location=3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform bool isTerrain;

out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coord;
out vec3 frag_color;

float random2D(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}
float lerp(float a, float b, float t) {
    return a + t*(b-a);
}
float noise2D(vec2 p) {
    vec2 ip = floor(p);
    vec2 f  = fract(p);
    float a = random2D(ip);
    float b = random2D(ip+vec2(1,0));
    float c = random2D(ip+vec2(0,1));
    float d = random2D(ip+vec2(1,1));
    float u = lerp(a,b,f.x);
    float v = lerp(c,d,f.x);
    return lerp(u,v,f.y);
}

void main()
{
    vec3 pos = v_position;
    if(isTerrain) {
        float h = noise2D(pos.xz * 0.2) * 2.0;
        pos.y += h;
    }

    frag_position = pos;
    frag_normal   = v_normal;
    frag_coord    = text_coord;
    frag_color    = v_color;

    gl_Position = Projection * View * Model * vec4(pos, 1);
}
