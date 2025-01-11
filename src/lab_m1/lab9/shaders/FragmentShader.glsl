#version 330

// Input
in vec2 texcoord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform int mixer;

// TODO(student): Declare various other uniforms
//in vec2 texcoord;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture2D() function.
    vec4 colour1 = texture2D(texture_1, texcoord);
	vec4 colour2 = texture2D(texture_2, texcoord);
	vec4 colour;
	if(mixer == 1){
		colour = mix(colour1,colour2,0.5f);
	}
	else
		colour = colour1;
	if(colour.a < 0.5f)
		discard;
	out_color = colour;

}