#version 330

in vec3 frag_position;
in vec3 frag_normal;
in vec2 frag_coord;
in vec3 frag_color;

layout(location = 0) out vec4 out_color;

uniform bool isTerrain;

float random2D(vec2 p) {
    return fract(sin(dot(p, vec2(12.9898,78.233))) * 43758.5453);
}
float lerp(float a, float b, float t) {
    return a + t*(b-a);
}
float noise2D(vec2 p) {
    vec2 ip=floor(p);
    vec2 f=fract(p);
    float a=random2D(ip);
    float b=random2D(ip+vec2(1,0));
    float c=random2D(ip+vec2(0,1));
    float d=random2D(ip+vec2(1,1));
    float u=lerp(a,b,f.x);
    float v=lerp(c,d,f.x);
    return lerp(u,v,f.y);
}

void main()
{
    if(isTerrain) {
        float n = noise2D(frag_position.xz * 0.2);
        vec3 cA = vec3(0.2, 0.8, 0.2);   // iarbă
        vec3 cB = vec3(0.5, 0.25, 0.0);  // pământ
        vec3 col = mix(cA, cB, n);
        out_color= vec4(col, 1.0);
    } else {
        out_color= vec4(frag_color, 1.0);
    }
}
